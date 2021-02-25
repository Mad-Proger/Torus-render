#include "Vector3.h"
#include "Torus.h"
#include "SDL.h"

#include <iostream>
#include <cmath>
#include <fstream>

#pragma comment(linker, "/SUBSYSTEM:Windows")
#pragma comment(linker, "/ENTRY:mainCRTStartup")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

struct renderThreadParameters {
	unsigned char *pixelArray;
	const Torus *torus;
	Vector3 lightSource;
	Vector3 viewPosition;
	long double fovAngle;

	unsigned int width;
	unsigned int height;

	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;

	SDL_sem *semRender;
	SDL_sem *semCopy;

	bool stopped;

	renderThreadParameters();
};

struct renderThread {
	SDL_Thread *threadPointer;

	SDL_sem *semRender;
	SDL_sem *semCopy;

	renderThreadParameters *threadParameters;

	renderThread();
	~renderThread();
};

int threadTorusRenderFunction(void *data);
renderThread *initializeThreads(const Torus *torus, const Vector3 &lightSource, const Vector3 &viewPosition, const long double &fovAngle, unsigned char *pixelArray, unsigned int width, unsigned int height, size_t partsWidth, size_t partsHeight);
void renderTorus(renderThread *threadArray, size_t partsWidth, size_t partsHeight);

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	std::ifstream parametersFile("StartParameters.txt");

	long double radius, distance, wx, wy, wz;
	Vector3 center;
	parametersFile >> radius >> distance >> center >> wx >> wy >> wz;
	Torus torus(center, radius, distance);

	Vector3 lightSource, viewPosition;
	long double fovAngle;
	parametersFile >> lightSource >> viewPosition >> fovAngle;
	fovAngle = M_PI * (fovAngle / 180.L);

	unsigned int width, height;
	parametersFile >> width >> height;

	size_t partsWidth, partsHeight;
	parametersFile >> partsWidth >> partsHeight;

	parametersFile.close();

	SDL_Window *wnd = SDL_CreateWindow("Torus Render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (wnd == nullptr) {
		SDL_Quit();
		return -1;
	}

	SDL_Renderer *rnd = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
	if (rnd == nullptr) {
		SDL_DestroyWindow(wnd);
		SDL_Quit();
		return -1;
	}

	SDL_Texture *torusTexture = SDL_CreateTexture(rnd, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (torusTexture == nullptr) {
		SDL_DestroyRenderer(rnd);
		SDL_DestroyWindow(wnd);
		SDL_Quit();
		return -1;
	}

	unsigned char *pixelArray = new unsigned char[4LL * width * height];
	bool windowOpened = true;

	renderThread *threadArray = initializeThreads(&torus, lightSource, viewPosition, fovAngle, pixelArray, width, height, partsWidth, partsHeight);

	while (windowOpened) {
		long double beg = clock();
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			if (evnt.type == SDL_QUIT) {
				windowOpened = false;
				break;
			}
		}

		SDL_RenderClear(rnd);
		renderTorus(threadArray, partsWidth, partsHeight);
		SDL_UpdateTexture(torusTexture, nullptr, pixelArray, 4 * width);
		SDL_RenderCopy(rnd, torusTexture, nullptr, nullptr);
		SDL_RenderPresent(rnd);

		torus.rotateX(wx);
		torus.rotateY(wy);
		torus.rotateZ(wz);

		SDL_Delay(20 - std::min(20, int(clock() - beg)));
	}

	SDL_DestroyTexture(torusTexture);
	SDL_DestroyRenderer(rnd);
	SDL_DestroyWindow(wnd);
	delete[] pixelArray;
	delete[] threadArray;

	SDL_Quit();
	return 0;
}

int threadTorusRenderFunction(void *data) {
	if (data == nullptr)
		return -1;
	renderThreadParameters *parameters = static_cast<renderThreadParameters *>(data);

	while (!parameters->stopped) {
		SDL_SemWait(parameters->semRender);
		if (parameters->stopped)
			break;
		parameters->torus->render(parameters->lightSource, parameters->viewPosition, parameters->fovAngle, parameters->pixelArray,
			parameters->width, parameters->height, parameters->left, parameters->top, parameters->right, parameters->bottom);
		SDL_SemPost(parameters->semCopy);
	}

	return 0;
}

renderThread *initializeThreads(const Torus *torus, const Vector3 &lightSource, const Vector3 &viewPosition, const long double &fovAngle, unsigned char *pixelArray, unsigned int width, unsigned int height, size_t partsWidth, size_t partsHeight) {
	renderThread *threadArray = new renderThread[partsWidth * partsHeight];

	size_t ind = 0;
	for (size_t i = 0; i < partsWidth; ++i) {
		for (size_t j = 0; j < partsHeight; ++j) {
			threadArray[ind].threadParameters = new renderThreadParameters();
			threadArray[ind].semRender = SDL_CreateSemaphore(0);
			threadArray[ind].semCopy = SDL_CreateSemaphore(0);

			threadArray[ind].threadParameters->torus = torus;
			threadArray[ind].threadParameters->pixelArray = pixelArray;
			threadArray[ind].threadParameters->lightSource = lightSource;
			threadArray[ind].threadParameters->viewPosition = viewPosition;
			threadArray[ind].threadParameters->fovAngle = fovAngle;

			threadArray[ind].threadParameters->width = width;
			threadArray[ind].threadParameters->height = height;
			threadArray[ind].threadParameters->left = (i * width) / partsWidth;
			threadArray[ind].threadParameters->top = (j * height) / partsHeight;
			threadArray[ind].threadParameters->right = ((i + 1) * width) / partsWidth;
			threadArray[ind].threadParameters->bottom = ((j + 1) * height) / partsHeight;

			threadArray[ind].threadParameters->semRender = threadArray[ind].semRender;
			threadArray[ind].threadParameters->semCopy = threadArray[ind].semCopy;

			threadArray[ind].threadPointer = SDL_CreateThread(threadTorusRenderFunction, "render thread", static_cast<void *>(threadArray[ind].threadParameters));

			++ind;
		}
	}

	return threadArray;
}

void renderTorus(renderThread *threadArray, size_t partsWidth, size_t partsHeight) {
	for (size_t i = 0; i < partsWidth * partsHeight; ++i)
		SDL_SemPost(threadArray[i].semRender);

	for (size_t i = 0; i < partsWidth * partsHeight; ++i)
		SDL_SemWait(threadArray[i].semCopy);
}

renderThreadParameters::renderThreadParameters() {
	this->torus = nullptr;
	this->pixelArray = nullptr;
	this->lightSource = Vector3();
	this->viewPosition = Vector3();
	this->fovAngle = 0.L;

	this->width = 0;
	this->height = 0;

	this->left = 0;
	this->top = 0;
	this->right = 0;
	this->bottom = 0;

	this->semRender = nullptr;
	this->semCopy = nullptr;

	this->stopped = false;
}

renderThread::renderThread() {
	this->threadPointer = nullptr;

	this->semRender = nullptr;
	this->semCopy = nullptr;

	this->threadParameters = nullptr;
}

renderThread::~renderThread() {
	this->threadParameters->stopped = true;
	SDL_SemPost(this->semRender);
	SDL_WaitThread(this->threadPointer, nullptr);

	SDL_DestroySemaphore(this->semRender);
	SDL_DestroySemaphore(this->semCopy);
	delete this->threadParameters;
}
