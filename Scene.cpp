#include "Scene.h"

using namespace std;

Scene::Scene(OpenGLClass* OpenGLObject, HWND hwnd) {
	handlerWindow = hwnd;
	OpenGL = OpenGLObject;
	Camera = 0;
	LightShader = 0;
	ColorShader = 0;
	ShaderBounding = 0;
	Triangulo = 0;
	Light = 0;
	speedAxisX = 0.0f;
	speedAxisY = 0.0f;
	speed = 0.0f;
	angulo = 0.0f;
	Object3d = 0;
	LoaderTexture = new TextureClass(OpenGL);
}

Scene::~Scene() {

}

bool Scene::Initialize() {
	bool result = true;
	string resultDetail = "";

	angulo = 0.0f;
	speedAxisX = SPEED_AXIS_X;
	speedAxisY = SPEED_AXIS_Y;
	speed = SPEED_CAMERA;

	// Crea un objeto camara.
	Camera = new CameraClass;
	if (!Camera) {
		result = false;
		_RPT1(0, "Alert! Camera has not been initialized. \n", 0);
		return result;
	}
	else {
		// Asigna las coordenadas de posicion y rotacion por defecto a los arreglos delta.
		DeltaPosition->X = InitialPosition[0]; DeltaPosition->Y = InitialPosition[1]; DeltaPosition->Z = InitialPosition[2];
		DeltaRotation->X = InitialRotation[0]; DeltaRotation->Y = InitialRotation[1]; DeltaRotation->Z = InitialRotation[2];
		// Luego se los pasamos a la camara.
		Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
		Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);
		// Con esto podriamos aislar siempre el punto de partida inicial de la escena para evitar teletrasportarse sin querer.
	}

	LightShader = new LightShaderClass(OpenGL, handlerWindow, "shaders/terrain.vs", "shaders/terrain.ps");
	if (!LightShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ColorShader = new ColorShaderClass(OpenGL, handlerWindow, "shaders/color.vs", "shaders/color.ps");
	if (!ColorShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the color shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! ColorShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	//Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, L"recursos/heightmap.jpg", "recursos/test.tga", "recursos/Piedras_normal.jpg",(float)400, (float)400, 0, 1);
	Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, L"recursos/heightmap.jpg", "recursos/chi.jpg", "recursos/arena.jpg", "recursos/heightmaparena.jpg", (float)400, (float)400);
	if (!Terreno){
		result = false;
		_RPT1(0, "Alert! Land has not been initialized. \n", 0);
		return result;
	}
	else {
		Terreno->SetLandShader(LightShader);
	}

	// Lago

	Lago = new Water(handlerWindow, OpenGL, LoaderTexture, L"recursos/AguaHMAP.png", "recursos/Agua.jpg", "recursos/Agua.jpg", "recursos/MapaAgua.png", (float)500, (float)500);
	if (!Lago) {
		result = false;
		_RPT1(0, "Alert! Land has not been initialized. \n", 0);
		return result;
	}
	else {
		Lago->SetLandShader(LightShader);
	}

	Triangulo = new TriangleClass(OpenGL);
	if (!Triangulo) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the triangle model object.", "Error", MB_OK);
		_RPT1(0, "Alert! Triangle has not been initialized. \n", 0);
		return result;
	}
	else {
		Triangulo->SetShaderTriangle(ColorShader);
	}

	ShaderModel = new ShaderTexture(OpenGL, handlerWindow, "shaders/light.vs", "shaders/light.ps");
	if (!ShaderModel) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader model object.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ShaderBounding = new ShaderBoundingBox(OpenGL, handlerWindow, "shaders/vertexcolor.vs", "shaders/vertexcolor.ps");
	if (!ShaderBounding) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader model object.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}


	// MODELOS


	Object3d = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/casaaba/casaab.obj",
		"recursos/casaaba/casaabcol.png");
	if (!Object3d) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d->SetShaders(ShaderModel, ShaderBounding);
	}

	Object3d1 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/caba1/caba1.obj",
		"recursos/caba1/una.jpg");
	if (!Object3d1) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d1->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d1->SetShaders(ShaderModel, ShaderBounding);
		//Object3d1->AddTexture("recursos/Official Models/Tree1T2.jpg"); // ¿Para multitexture?
	}

	Object3d2 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/caba2/caba2.obj",
		"recursos/caba2/una.png");
	if (!Object3d2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d2->SetShaders(ShaderModel, ShaderBounding);
	}

	Object3d3 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/senal/senal.obj",
		"recursos/senal/senal.png");
	if (!Object3d3) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d3->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d3->SetShaders(ShaderModel, ShaderBounding);
	}

	Object3d4 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/puente2/puente2.obj",
		"recursos/puente2/puente2.png");
	if (!Object3d4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d4->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d4->SetShaders(ShaderModel, ShaderBounding);
	}

	Object3d5 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/manzana/manzana.obj",
		"recursos/manzana/una.png");
	if (!Object3d5) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d5->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d5->SetShaders(ShaderModel, ShaderBounding);
	}

	Object3d6 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/pelicano/pelicano.obj",
		"recursos/pelicano/pelicano.jpg");
	if (!Object3d6) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d6->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d6->SetShaders(ShaderModel, ShaderBounding);
	}


	Object3d7 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/piedra/pied.obj",
		"recursos/piedra/pied.jpg");
	if (!Object3d7) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d7->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d7->SetShaders(ShaderModel, ShaderBounding);
	}



	Object3d8 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/puente/puen.obj",
		"recursos/puente/una.png");
	if (!Object3d8) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d8->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d8->SetShaders(ShaderModel, ShaderBounding);
	}


	Object3d9 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/mango/mango.obj",
		"recursos/mango/una.jpg",
		"recursos/mango/mannor.jpg");
	if (!Object3d9) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d9->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d9->SetShaders(ShaderModel, ShaderBounding);
	}


	Pigeon = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/pigeon/pigeon.obj",
		"recursos/paloma.png");
	if (!Pigeon) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Pigeon->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Pigeon->SetShaders(ShaderModel, ShaderBounding);
	}

	Casa2 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/puente2/puente22.obj",
		"recursos/caba2/una.png");
	if (!Casa2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Casa2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Casa2->SetShaders(ShaderModel, ShaderBounding);
	}

	pelicano2 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/pelicano/pelicano2.obj",
		"recursos/pelicano/pelicano.jpg");
	if (!pelicano2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = pelicano2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		pelicano2->SetShaders(ShaderModel, ShaderBounding);
	}

	pelicano3 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/pelicano/pelicano3.obj",
		"recursos/pelicano/pelicano.jpg");
	if (!pelicano3) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = pelicano3->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		pelicano3->SetShaders(ShaderModel, ShaderBounding);
	}

	pelicano4 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/pelicano/pelicano3.obj",
		"recursos/pelicano/pelicano.jpg");
	if (!pelicano4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = pelicano4->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		pelicano4->SetShaders(ShaderModel, ShaderBounding);
	}

	plaza = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/fuente/fuente.obj",
		"recursos/fuente/fuente.png");
	if (!plaza) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = plaza->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		plaza->SetShaders(ShaderModel, ShaderBounding);
	}

	piedra2 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/piedra/pied.obj",
		"recursos/piedra/pied.jpg");
	if (!piedra2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = piedra2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		piedra2->SetShaders(ShaderModel, ShaderBounding);
	}

	piedra3 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/piedra/pied.obj",
		"recursos/piedra/pied.jpg");
	if (!piedra3) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = piedra3->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		piedra3->SetShaders(ShaderModel, ShaderBounding);
	}


	piedra4 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/piedra/pied.obj",
		"recursos/piedra/pied2.jpg");
	if (!piedra4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = piedra4->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		piedra4->SetShaders(ShaderModel, ShaderBounding);
	}

	piedra5 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/piedra/pied.obj",
		"recursos/piedra/pied2.jpg");
	if (!piedra5) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = piedra5->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		piedra5->SetShaders(ShaderModel, ShaderBounding);
	}

	piedra6 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/piedra/pied.obj",
		"recursos/piedra/pied2.jpg");
	if (!piedra6) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = piedra6->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		piedra6->SetShaders(ShaderModel, ShaderBounding);
	}

	silla = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/playa/chair_01.obj",
		"recursos/playa/chair_01.png");
	if (!silla) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = silla->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		silla->SetShaders(ShaderModel, ShaderBounding);
	}

	parasol = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/playa/parasol.obj",
		"recursos/playa/parasol.png");
	if (!parasol) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = parasol->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		parasol->SetShaders(ShaderModel, ShaderBounding);
	}

	pelota = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/playa/ball.obj",
		"recursos/playa/ball.png");
	if (!pelota) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = pelota->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		pelota->SetShaders(ShaderModel, ShaderBounding);
	}

	hielera = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/playa/cooler.obj",
		"recursos/playa/cooler.png");
	if (!hielera) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = hielera->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		hielera->SetShaders(ShaderModel, ShaderBounding);
	}

	flotador = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/playa/float_01.obj",
		"recursos/playa/float_01.png");
	if (!flotador) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = flotador->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		flotador->SetShaders(ShaderModel, ShaderBounding);
	}

	mesa = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/playa/mesa.obj",
		"recursos/playa/mesa.png");
	if (!mesa) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = mesa->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		mesa->SetShaders(ShaderModel, ShaderBounding);
	}

	silla1 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/playa/chair_02.obj",
		"recursos/playa/chair_02.png");
	if (!silla1) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = silla1->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		silla1->SetShaders(ShaderModel, ShaderBounding);
	}

	juguete = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/playa/toy.obj",
		"recursos/playa/toy.png");
	if (!juguete) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = juguete->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		juguete->SetShaders(ShaderModel, ShaderBounding);
	}


	manza2 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/manzana/manzana.obj",
		"recursos/manzana/una.png");
	if (!manza2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = manza2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		manza2->SetShaders(ShaderModel, ShaderBounding);
	}

	manza3 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/manzana/manzana.obj",
		"recursos/manzana/una.png");
	if (!manza3) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = manza3->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		manza3->SetShaders(ShaderModel, ShaderBounding);
	}

	manza4 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/manzana/manzana.obj",
		"recursos/manzana/una.png");
	if (!manza4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = manza4->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		manza4->SetShaders(ShaderModel, ShaderBounding);
	}

	mango2 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/mango/mango.obj",
		"recursos/mango/una.jpg",
		"recursos/mango/mannor.jpg");
	if (!mango2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = mango2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		mango2->SetShaders(ShaderModel, ShaderBounding);
	}

	mango3 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/mango/mango.obj",
		"recursos/mango/una.jpg",
		"recursos/mango/mannor.jpg");
	if (!mango3) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = mango3->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		mango3->SetShaders(ShaderModel, ShaderBounding);
	}

	mango4 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/mango/mango.obj",
		"recursos/mango/una.jpg",
		"recursos/mango/mannor.jpg");
	if (!mango4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = mango4->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		mango4->SetShaders(ShaderModel, ShaderBounding);
	}




	// Skydome

	ShaderSky = new SkydomeShaderClass(OpenGL, handlerWindow, "shaders/SkydomeShader.vs", "shaders/SkydomeShader.ps");
	if (!ShaderSky) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader skydome.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	Skydome = new Dome("recursos/sky.jpg", "recursos/sky2.jpg", "recursos/sky3.jpg", OpenGL, LoaderTexture, 500);
	if (!Skydome) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the Skydome.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Skydome->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the Skydome.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Skydome->SetIterpolationDay(false, true);
		Skydome->SetShader(ShaderSky);
	}

	// Collision Boxes
	box = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 10.0f, 15.0f, 8.0f }, BoundingBox::GLFLOAT3{ -10.0f, -15.0f, -8.0f });
	if (!box) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box->SetShader(ShaderBounding);
	}

	

	//box3 = new BoundingBoxRenderer(OpenGL,
	//	BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	//if (!box3) {
	//	result = false;
	//	MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
	//	_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
	//	return result;
	//}
	//else {
	//	box3->SetShader(ShaderBounding);
	//}

	//box4 = new BoundingBoxRenderer(OpenGL,
	//	BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	//if (!box4) {
	//	result = false;
	//	MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
	//	_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
	//	return result;
	//}
	//else {
	//	box4->SetShader(ShaderBounding);
	//}

	//box5 = new BoundingBoxRenderer(OpenGL,
	//	BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	//if (!box5) {
	//	result = false;
	//	MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
	//	_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
	//	return result;
	//}
	//else {
	//	box5->SetShader(ShaderBounding);
	//}

	//box6 = new BoundingBoxRenderer(OpenGL,
	//	BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	//if (!box6) {
	//	result = false;
	//	MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
	//	_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
	//	return result;
	//}
	//else {
	//	box6->SetShader(ShaderBounding);
	//}



	// Billboards
	ShaderBill = new BillboardShaderClass(OpenGL, handlerWindow, "shaders/billboard.vs", "shaders/billboard.ps");
	if (!ShaderBill) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	arbol2D = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/abolit.png");
	if (!arbol2D) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		arbol2D->Initialize(10.0f);
		arbol2D->SetShader(ShaderBill);
	}

	piedra2d = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Piedra.png");
	if (!piedra2d) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		piedra2d->Initialize(5.0f);
		piedra2d->SetShader(ShaderBill);
	}

	arbusto = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Arbusto.png");
	if (!arbusto) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		arbusto->Initialize(6.0f);
		arbusto->SetShader(ShaderBill);
	}

	flores = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/flores.png");
	if (!flores) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		flores->Initialize(6.0f);
		flores->SetShader(ShaderBill);
	}

	maceta = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Macetaa.png");
	if (!maceta) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		maceta->Initialize(2.0f);
		maceta->SetShader(ShaderBill);
	}

	return result;
}

bool Scene::Render() {
	bool result = true;

	// Limpia los buffers para iniciar la escena nueva.
	OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Definimos nuestro espacio para las 3 matrices necesarias para renderizar.
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];

	// Actualiza la posicion y rotacion de la camara
	Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
	Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);

	// Genera la matriz de vista a partir de la posicion de la camara.
	Camera->Render();

	// Obtiene las matrices de vista, mundo y proyeccion de opengl y el objeto de la camara.
	OpenGL->GetWorldMatrix(worldMatrix);
	Camera->GetViewMatrix(viewMatrix);
	OpenGL->GetProjectionMatrix(projectionMatrix);

	// Renderizamos el Skydome
	//Skydome->CicloInterpolaciones();
	float CycleSpeed = 0.001f;
	static float t =0.0f;
	static float dia=0.6f;
	int opcion = 1;	
	if (dia > 1.8) {
		dia = 0.0f;
	}
	dia += CycleSpeed;
	//noche
	if (dia > 0.0 && dia <= 0.4) {
		opcion = 5;
		t = 0.0f;
	}
	//noche-dia
	if (dia > 0.4 && dia <= 0.6) {
		opcion = 6;
		t += 0.005;
	}
	//dia
	if (dia > 0.6 && dia <=1.0){
		opcion = 1;	
		t = 0.0f;
	}

	//dia-tarde
	if (dia > 1.0 && dia <= 1.2) {
		opcion = 2;
		t += 0.005;
	}
		//tarde
	if (dia > 1.2 && dia <= 1.6){
		opcion = 3;
		t = 0.0f;
	}

	//tarde-noche
	if (dia > 1.6){
		opcion = 4;
		t += 0.005;
	}

	//noche
	// Renderizamos el Skydome
	Skydome->Render(viewMatrix, projectionMatrix, 1.0-t, opcion);

	// Renderizamos terreno
	Terreno->Render(OpenGL, worldMatrix, viewMatrix, projectionMatrix);
	Lago->Render(OpenGL, viewMatrix, projectionMatrix);


	// Renderizamos Triangulo (ya se logra ver en escena)
	Triangulo->Render(viewMatrix, projectionMatrix);

	// Renderizamos nuestros objetos en la escena
	Object3d->Render(viewMatrix, projectionMatrix, false);
	Object3d1->Render(viewMatrix, projectionMatrix, true);
	Object3d2->Render(viewMatrix, projectionMatrix, true);
	Object3d3->Render(viewMatrix, projectionMatrix, true);
	Object3d4->Render(viewMatrix, projectionMatrix, true);
	Object3d5->Render(viewMatrix, projectionMatrix, true);
	Object3d6->Render(viewMatrix, projectionMatrix, true);
	Object3d7->Render(viewMatrix, projectionMatrix, true);
	Object3d8->Render(viewMatrix, projectionMatrix, true);
	Object3d9->Render(viewMatrix, projectionMatrix, true);
	Pigeon->Render(viewMatrix, projectionMatrix, false);
	Casa2->Render(viewMatrix, projectionMatrix, true);

	pelicano2->Render(viewMatrix, projectionMatrix, true);
	pelicano3->Render(viewMatrix, projectionMatrix, true);
	pelicano4->Render(viewMatrix, projectionMatrix, true);
	plaza->Render(viewMatrix, projectionMatrix, true);

	piedra2->Render(viewMatrix, projectionMatrix, true);
	piedra3->Render(viewMatrix, projectionMatrix, true);
	piedra4->Render(viewMatrix, projectionMatrix, true);
	piedra5->Render(viewMatrix, projectionMatrix, true);
	piedra6->Render(viewMatrix, projectionMatrix, true);

	silla->Render(viewMatrix, projectionMatrix, true);
	parasol->Render(viewMatrix, projectionMatrix, true);
	pelota->Render(viewMatrix, projectionMatrix, true);
	hielera->Render(viewMatrix, projectionMatrix, true);
	flotador->Render(viewMatrix, projectionMatrix, true);
	mesa->Render(viewMatrix, projectionMatrix, true);
	silla1->Render(viewMatrix, projectionMatrix, true);
	juguete->Render(viewMatrix, projectionMatrix, true);


	manza2->Render(viewMatrix, projectionMatrix, true);
	manza3->Render(viewMatrix, projectionMatrix, true);
	manza4->Render(viewMatrix, projectionMatrix, true);
	mango2->Render(viewMatrix, projectionMatrix, true);
	mango3->Render(viewMatrix, projectionMatrix, true);
	mango4->Render(viewMatrix, projectionMatrix, true);



	// Renderizamos las cajas de colisi�n
	box->Draw(viewMatrix, projectionMatrix);
	/*box3->Draw(viewMatrix, projectionMatrix);
	box4->Draw(viewMatrix, projectionMatrix);
	box5->Draw(viewMatrix, projectionMatrix);
	box6->Draw(viewMatrix, projectionMatrix);*/

	// Renderizamos los billboards
	arbol2D->Render(viewMatrix, projectionMatrix, 
		-108.0f, Terreno->Superficie(22.0f, 22.0f), 68.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-99.0f, Terreno->Superficie(22.0f, 22.0f), 83.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-89.0f, Terreno->Superficie(22.0f, 22.0f), 94.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-88.0f, Terreno->Superficie(22.0f, 22.0f), 102.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-75.0f, Terreno->Superficie(22.0f, 22.0f), 94.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-61.0f, Terreno->Superficie(22.0f, 22.0f), 106.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-60.0f, Terreno->Superficie(22.0f, 22.0f), 91.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-49.0f, Terreno->Superficie(25.0f, 25.0f), 67.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-37.0f, Terreno->Superficie(25.0f, 25.0f), 80.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-39.0f, Terreno->Superficie(25.0f, 25.0f), 92.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-49.0f, Terreno->Superficie(25.0f, 25.0f), 90.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-46.0f, Terreno->Superficie(25.0f, 25.0f), 103.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-39.0f, Terreno->Superficie(25.0f, 25.0f), 111.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-26.0f, Terreno->Superficie(25.0f, 25.0f), 118.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-13.0f, Terreno->Superficie(25.0f, 25.0f), 107.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-13.0f, Terreno->Superficie(25.0f, 25.0f), 107.0f,
		DeltaPosition->X, DeltaPosition->Z);


	arbol2D->Render(viewMatrix, projectionMatrix,
		-3.0f, Terreno->Superficie(25.0f, 25.0f), 92.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-8.0f, Terreno->Superficie(25.0f, 25.0f), 128.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-2.0f, Terreno->Superficie(25.0f, 25.0f), 119.0f,
		DeltaPosition->X, DeltaPosition->Z);

	// aqui van 18

	arbol2D->Render(viewMatrix, projectionMatrix,
		-15.0f, Terreno->Superficie(25.0f, 25.0f), 73.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		-5.0f, Terreno->Superficie(25.0f, 25.0f), 71.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		2.0f, Terreno->Superficie(25.0f, 25.0f), 73.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		4.0f, Terreno->Superficie(28.0f, 28.0f), 61.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		7.0f, Terreno->Superficie(31.0f, 31.0f), 52.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		15.0f, Terreno->Superficie(31.0f, 31.0f), 60.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		27.0f, Terreno->Superficie(31.0f, 31.0f), 62.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		21.0f, Terreno->Superficie(28.0f, 28.0f), 87.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		20.0f, Terreno->Superficie(28.0f, 28.0f), 100.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		23.0f, Terreno->Superficie(28.0f, 28.0f), 109.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		34.0f, Terreno->Superficie(28.0f, 28.0f), 102.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		37.0f, Terreno->Superficie(28.0f, 28.0f), 111.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		35.0f, Terreno->Superficie(28.0f, 28.0f), 121.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		46.0f, Terreno->Superficie(28.0f, 28.0f), 113.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		38.0f, Terreno->Superficie(28.0f, 28.0f), 95.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix,
		27.0f, Terreno->Superficie(28.0f, 28.0f), 87.0f,
		DeltaPosition->X, DeltaPosition->Z);


	arbol2D->Render(viewMatrix, projectionMatrix,
		13.0f, Terreno->Superficie(28.0f, 28.0f), 74.0f,
		DeltaPosition->X, DeltaPosition->Z);

	// flores


	flores->Render(viewMatrix, projectionMatrix,
		-91.0f, Terreno->Superficie(18.0f, 18.0f), -80.0f,
		DeltaPosition->X, DeltaPosition->Z);


	flores->Render(viewMatrix, projectionMatrix,
		-55.0f, Terreno->Superficie(18.0f, 18.0f), -117.0f,
		DeltaPosition->X, DeltaPosition->Z);


	flores->Render(viewMatrix, projectionMatrix,
		-28.0f, Terreno->Superficie(18.0f, 18.0f), -113.0f,
		DeltaPosition->X, DeltaPosition->Z);


	flores->Render(viewMatrix, projectionMatrix,
		-2.0f, Terreno->Superficie(18.0f, 18.0f), -58.0f,
		DeltaPosition->X, DeltaPosition->Z);

	flores->Render(viewMatrix, projectionMatrix,
		-8.0f, Terreno->Superficie(24.0f, 24.0f), -17.0f,
		DeltaPosition->X, DeltaPosition->Z);

	// arbustos

	arbusto->Render(viewMatrix, projectionMatrix,
		2.0f, Terreno->Superficie(18.0f, 18.0f), -49.0f,
		DeltaPosition->X, DeltaPosition->Z);


	arbusto->Render(viewMatrix, projectionMatrix,
		-41.0f, Terreno->Superficie(17.0f, 17.0f), -104.0f,
		DeltaPosition->X, DeltaPosition->Z);


	arbusto->Render(viewMatrix, projectionMatrix,
		-82.0f, Terreno->Superficie(17.0f, 17.0f), -88.0f,
		DeltaPosition->X, DeltaPosition->Z);


	arbusto->Render(viewMatrix, projectionMatrix,
		-86.0f, Terreno->Superficie(18.0f, 18.0f), -60.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbusto->Render(viewMatrix, projectionMatrix,
		0.0f, Terreno->Superficie(28.0f, 28.0f), -32.0f,
		DeltaPosition->X, DeltaPosition->Z);



	arbusto->Render(viewMatrix, projectionMatrix,
		59.0f, Terreno->Superficie(22.0f, 22.0f), -17.0f,
		DeltaPosition->X, DeltaPosition->Z);


	arbusto->Render(viewMatrix, projectionMatrix,
		66.0f, Terreno->Superficie(19.0f, 19.0f), -51.0f,
		DeltaPosition->X, DeltaPosition->Z);


	arbusto->Render(viewMatrix, projectionMatrix,
		78.0f, Terreno->Superficie(18.0f, 18.0f), -47.0f,
		DeltaPosition->X, DeltaPosition->Z);


	arbusto->Render(viewMatrix, projectionMatrix,
		86.0f, Terreno->Superficie(17.0f, 17.0f), -47.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbusto->Render(viewMatrix, projectionMatrix,
		89.0f, Terreno->Superficie(23.0f, 23.0f), -27.0f,
		DeltaPosition->X, DeltaPosition->Z);

	arbusto->Render(viewMatrix, projectionMatrix,
		74.0f, Terreno->Superficie(27.0f, 27.0f), -24.0f,
		DeltaPosition->X, DeltaPosition->Z);

	// Maceta

	maceta->Render(viewMatrix, projectionMatrix,
		-21.0f, Terreno->Superficie(30.0f, 30.0f), -40.0f,
		DeltaPosition->X, DeltaPosition->Z);


	maceta->Render(viewMatrix, projectionMatrix,
		-19.0f, Terreno->Superficie(28.0f, 28.0f), -53.0f,
		DeltaPosition->X, DeltaPosition->Z);


	maceta->Render(viewMatrix, projectionMatrix,
		-22.0f, Terreno->Superficie(28.0f, 28.0f), -67.0f,
		DeltaPosition->X, DeltaPosition->Z);


	maceta->Render(viewMatrix, projectionMatrix,
		-34.0f, Terreno->Superficie(28.0f, 28.0f), -79.0f,
		DeltaPosition->X, DeltaPosition->Z);

	maceta->Render(viewMatrix, projectionMatrix,
		-55.0f, Terreno->Superficie(27.0f, 27.0f), -77.0f,
		DeltaPosition->X, DeltaPosition->Z);



	maceta->Render(viewMatrix, projectionMatrix,
		-68.0f, Terreno->Superficie(28.5f, 28.5f), -70.0f,
		DeltaPosition->X, DeltaPosition->Z);


	maceta->Render(viewMatrix, projectionMatrix,
		-69.0f, Terreno->Superficie(28.5f, 28.5f), -52.0f,
		DeltaPosition->X, DeltaPosition->Z);


	maceta->Render(viewMatrix, projectionMatrix,
		-61.0f, Terreno->Superficie(29.8f, 29.8f), -34.0f,
		DeltaPosition->X, DeltaPosition->Z);

	// piedras

	piedra2d->Render(viewMatrix, projectionMatrix,
		74.0f, Terreno->Superficie(26.0f, 26.0f), -2.0f,
		DeltaPosition->X, DeltaPosition->Z);

	piedra2d->Render(viewMatrix, projectionMatrix,
		100.0f, Terreno->Superficie(25.0f, 25.0f), 12.0f,
		DeltaPosition->X, DeltaPosition->Z);

	piedra2d->Render(viewMatrix, projectionMatrix,
		129.0f, Terreno->Superficie(19.0f, 19.0f), -3.5f,
		DeltaPosition->X, DeltaPosition->Z);

	piedra2d->Render(viewMatrix, projectionMatrix,
		108.0f, Terreno->Superficie(19.0f, 19.0f), -25.0f,
		DeltaPosition->X, DeltaPosition->Z);

	piedra2d->Render(viewMatrix, projectionMatrix,
		101.0f, Terreno->Superficie(19.0f, 19.0f), -50.0f,
		DeltaPosition->X, DeltaPosition->Z);

	piedra2d->Render(viewMatrix, projectionMatrix,
	   110.0f, Terreno->Superficie(19.0f, 19.0f), -67.0f,
		DeltaPosition->X, DeltaPosition->Z);

	piedra2d->Render(viewMatrix, projectionMatrix,
		125.0f, Terreno->Superficie(19.0f, 19.0f), -80.0f,
		DeltaPosition->X, DeltaPosition->Z);

	piedra2d->Render(viewMatrix, projectionMatrix,
		109.0f, Terreno->Superficie(24.0f, 24.0f), -96.0f,
		DeltaPosition->X, DeltaPosition->Z);

	piedra2d->Render(viewMatrix, projectionMatrix,
		85.0f, Terreno->Superficie(25.0f, 25.0f), -91.0f,
		DeltaPosition->X, DeltaPosition->Z);

	piedra2d->Render(viewMatrix, projectionMatrix,
		73.0f, Terreno->Superficie(26.0f, 26.0f), -62.0f,
		DeltaPosition->X, DeltaPosition->Z);

	


	// Damos la instruccion de que termino la escena para que nos muestre frame a frame.
	OpenGL->EndScene();

	return result;
}

bool Scene::Update(InputClass* input, float deltaTime) {
	bool result = true;

	this->input = input;
	this->deltaTime = deltaTime;
	Lago->animateWater(OpenGL);
	result = ManageCommands();
	if (!result) {
		result = false;
		_RPT1(0, "Alert! Error on key. \n", 0);
		return result;
	}

	float* matrixGameObject = Object3d->GetWorldMatrix(); // casaabd
	OpenGL->MatrixTranslation(matrixGameObject, 75.0f, 18.0f, -33.0f);

	float* matrixGameObject1 = Object3d1->GetWorldMatrix(); // cabañagrande
	OpenGL->MatrixTranslation(matrixGameObject1, -83.0f, 19.0f, -31.0f);

	float* matrixGameObject2 = Object3d2->GetWorldMatrix(); // cabañachiquita
	OpenGL->MatrixTranslation(matrixGameObject2, -85.0f, 13.0f, -109.0f);

	float* matrixGameObject3 = Object3d3->GetWorldMatrix(); //senal
	OpenGL->MatrixTranslation(matrixGameObject3, 63.0f, 18.0f, -30.0f);

	float* matrixGameObject4 = Object3d4->GetWorldMatrix(); // puente normal
	OpenGL->MatrixTranslation(matrixGameObject4, 29.0f, 16.0f, -44.0f);

	float* matrixGameObject6 = Object3d6->GetWorldMatrix(); // pelicano
	OpenGL->MatrixTranslation(matrixGameObject6, -95.f, 17.0f, -48.0f);

	float* matrixGameObject7 = Object3d7->GetWorldMatrix(); // piedra
	OpenGL->MatrixTranslation(matrixGameObject7, 64.f, 20.0f, 3.0f);

	float* matrixGameObject8 = Object3d8->GetWorldMatrix(); // puente bonito
	OpenGL->MatrixTranslation(matrixGameObject8, -56.f, 15.5f, 13.0f);

	

	float* matrixGameObject10 = Casa2->GetWorldMatrix(); // casa2
	OpenGL->MatrixTranslation(matrixGameObject10, 2.0f, 16.0f, -90.0f);


	float* matrixGameObject11 = pelicano2->GetWorldMatrix(); // pelicano
	OpenGL->MatrixTranslation(matrixGameObject11, -69.0f, 14.5f, -118.0f);

	float* matrixGameObject12 = pelicano3->GetWorldMatrix(); 
	OpenGL->MatrixTranslation(matrixGameObject12, 4.0f, 17.0f, -69.0f);

	float* matrixGameObject13 = pelicano4->GetWorldMatrix(); 
	OpenGL->MatrixTranslation(matrixGameObject13, -34.0f, 17.0f, -41.0f);

	float* matrixGameObject14 = plaza->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject14, -44.0f, 15.5f, -54.0f);

	// piedras
	float* matrixGameObject15 = piedra2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject15, 100.0f, 16.5f, -8.0f);

	float* matrixGameObject16 = piedra3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject16, 118.0f, 15.4f, -34.0f);

	float* matrixGameObject17 = piedra4->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject17, 85.0f, 15.3f, -36.0f);

	float* matrixGameObject18 = piedra5->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject18, 111.0f, 18.5f, 61.0f);

	float* matrixGameObject19 = piedra6->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject19, 87.0f, 17.2f, -70.0f);

	// playa

	float* matrixGameObject20 = silla->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject20, 172.0f, 7.8f, 62.0f);

	float* matrixGameObject21 = parasol->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject21, 174.0f, 8.0f, 58.0f);

	float* matrixGameObject22 = pelota->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject22, 181.0f, 9.0f, 64.0f);

	float* matrixGameObject23 = hielera->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject23, 181.0f, 8.0f, 77.0f);

	/*float* matrixGameObject23 = hielera->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject23, 181.0f, 7.5f, 77.0f);*/

	float* matrixGameObject24 = flotador->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject24, 191.0f, 9.0f, 62.0f);

	//float* matrixGameObject24 = flotador->GetWorldMatrix();
	//OpenGL->MatrixTranslation(matrixGameObject24, 191.0f, 7.8f, 62.0f);

	float* matrixGameObject25 = mesa->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject25, 176.0f, 7.0f, 76.0f);

	float* matrixGameObject26 = silla1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject26, 173.0f, 7.8f, 56.0f);

	float* matrixGameObject27 = juguete->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject27, 184.0f, 7.8f, 50.0f);

	// manzanas

	float* matrixGameObject5 = Object3d5->GetWorldMatrix(); // manzana
	OpenGL->MatrixTranslation(matrixGameObject5, -61.0f, 14.0f, 76.0f);

	float* matrixGameObject28 = manza2->GetWorldMatrix(); // manzana
	OpenGL->MatrixTranslation(matrixGameObject28, -21.0f, 15.6f, 87.0f);

	float* matrixGameObject29 = manza3->GetWorldMatrix(); // manzana
	OpenGL->MatrixTranslation(matrixGameObject29, -46.0f, 16.5f, 112.0f);

	float* matrixGameObject30 = manza4->GetWorldMatrix(); // manzana
	OpenGL->MatrixTranslation(matrixGameObject30, -103.0f, 13.8f, 77.0f);


	// mangos

	float* matrixGameObject9 = Object3d9->GetWorldMatrix(); // mango
	OpenGL->MatrixTranslation(matrixGameObject9, -80.0f, 15.0f, 73.0f);


	float* matrixGameObject31 = mango2->GetWorldMatrix(); 
	OpenGL->MatrixTranslation(matrixGameObject31, -49.0f, 17.0f, 107.0f);


	float* matrixGameObject32 = mango3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject32, 20.0f, 16.0f, 117.0f);


	float* matrixGameObject33 = mango4->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject33, 34.0f, 18.4f, 78.0f);


	float* matrixAirplane = Pigeon->GetWorldMatrix();
	float* matrixAux = new float[16];
	float* matrixAux2 = new float[16];
	float* matrixAux3 = new float[16];
	OpenGL->BuildIdentityMatrix(matrixAux);
	OpenGL->BuildIdentityMatrix(matrixAux2);
	OpenGL->BuildIdentityMatrix(matrixAux3);
	OpenGL->MatrixTranslation(matrixAux, DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
	OpenGL->MatrixRotationX(matrixAux3, DeltaRotation->X * 0.0174532925f);
	OpenGL->MatrixRotationY(matrixAux2, DeltaRotation->Y * 0.0174532925f);
	OpenGL->MatrixMultiply(matrixAirplane, matrixAux3, matrixAux2);
	OpenGL->MatrixMultiply(matrixAirplane, matrixAirplane, matrixAux);

	//Tranformaciones de cajas de colisi�n
	float* auxMatrix = new float[16]{ 0.0f };
	OpenGL->BuildIdentityMatrix(auxMatrix);

	float* matrixBox = box->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox, 74.0f, 20.0f, -36.0f);
	OpenGL->MatrixScale(auxMatrix, 1.0f, 1.0f, 1.0f);
	OpenGL->MatrixMultiply(matrixBox, matrixBox, auxMatrix);
	//Colisi�n por esfera
	/*if (Object3d->GetSphericalCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z, 2)) {
		MessageBox(handlerWindow, L"Colisionando", L"Aviso", MB_OK);
	}*/

	//Colisi�n por caja
	
	if (Object3d1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Object3d2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Object3d3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Object3d4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Object3d8->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}


	if (Casa2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (pelicano2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Object3d6->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (pelicano3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (pelicano4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}


	if (plaza->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Object3d7->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (piedra2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (piedra6->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (piedra3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (piedra4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (piedra5->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (silla->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (parasol->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (pelota->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (hielera->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (flotador->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (mesa->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (silla1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (juguete->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	// mangos

	if (mango2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (mango3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (mango4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Object3d9->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	// manzanas

	if (manza2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (manza3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (manza4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Object3d5->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}






	if (box->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	return result;
}

bool Scene::ManageCommands() {
	bool result = true;
	
	//Guardar la posici�n de la C�mara antes de actualizarla
	LastDeltaPosition->X = DeltaPosition->X;
	LastDeltaPosition->Y = DeltaPosition->Y;
	LastDeltaPosition->Z = DeltaPosition->Z;

	float vr[2]{ 0,0 };
	float radians = DeltaRotation->Y * 0.0174532925f;


	

		if ((input->GetKey(KeyCode.W) || input->GetKey(KeyCode.Up))) {
			vr[0] += sinf(radians) * speed * deltaTime;
			vr[1] += cosf(radians) * speed * deltaTime;
		}
		if ((input->GetKey(KeyCode.S) || input->GetKey(KeyCode.Down))) {
			vr[0] -= sinf(radians) * speed * deltaTime;
			vr[1] -= cosf(radians) * speed * deltaTime;
		}
		if ((input->GetKey(KeyCode.A) || input->GetKey(KeyCode.Left))) {
			vr[0] -= sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
			vr[1] -= cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		}
		if ((input->GetKey(KeyCode.D) || input->GetKey(KeyCode.Right))) {
			vr[0] += sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
			vr[1] += cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		}

		if (input->GetRightAxisY() || input->GetRightAxisX()) {
			float angleX = input->GetRightAxisY() * speedAxisX * deltaTime;
			float angleY = input->GetRightAxisX() * speedAxisY * deltaTime;
			DeltaRotation->X -= angleX;
			DeltaRotation->Y += angleY;
		}

		if (input->GetKey(KeyCode.I)) {
			if ((DeltaRotation->X - (speedAxisX * deltaTime)) > (-30)) {
				DeltaRotation->X -= speedAxisX * deltaTime;
			}

		}
		if (input->GetKey(KeyCode.K)) {
			if ((DeltaRotation->X + (speedAxisX * deltaTime)) < (30)) {
				DeltaRotation->X += speedAxisX * deltaTime;
			}
		}
		if (input->GetKey(KeyCode.J)) {
			DeltaRotation->Y -= speedAxisY * deltaTime;
		}
		if (input->GetKey(KeyCode.L)) {
			DeltaRotation->Y += speedAxisY * deltaTime;
		}
		if (input->GetKey(KeyCode.One)) {
			Object3d1->ChangeTexture(0, 0);
		}
		if (input->GetKey(KeyCode.Two)) {
			Object3d1->ChangeTexture(1, 1);
		}
		if (input->GetKey(KeyCode.Three)) {
			Object3d1->ChangeTexture(2, 2);
		}
		if (input->GetKey(KeyCode.Four)) {
			Object3d1->ChangeTexture(3, 3);
		}
		if (input->GetKey(KeyCode.Five)) {
			Object3d1->ChangeTexture(4, 4);
		}
		if (input->GetKey(KeyCode.Six)) {
			Object3d1->ChangeTexture(5, 5);
		}

		if ((input->GetKey(KeyCode.P))) {
			string info = "X: " + to_string(DeltaPosition->X) + " Y: " + to_string(DeltaPosition->Y) + " Z: " + to_string(DeltaPosition->Z);
			MessageBoxA(handlerWindow, info.c_str(), "chi", MB_OK);
		}

		if (vr[0] == 0 && toivolandopapito == true)
		{
			if (!input->GetKey(KeyCode.Shift))
			{
				vr[0] += sinf(radians) * speed * deltaTime;
				vr[1] += cosf(radians) * speed * deltaTime;
			}
			else
			{
				vr[0] += sinf(radians) * speed * 2 * deltaTime;
				vr[1] += cosf(radians) * speed * 2 * deltaTime;
			}
		}

		if (DeltaRotation->X < -10)
		{
			DeltaPosition->Y += 0.125f + (-DeltaRotation->X * .005);
			toivolandopapito = true;
		}
		else
		{
			if (DeltaPosition->Y >= Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2 && toivolandopapito == true)
			{
				DeltaPosition->Y -= (0.05f + (DeltaRotation->X * .0125));
			}
			else
			{
				DeltaPosition->Y = Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2;
				toivolandopapito = false;
			}
		}


	if (DeltaPosition->X + vr [0] >= 198 || DeltaPosition->X + vr[0] <= -198) {

		DeltaPosition->X = LastDeltaPosition->X;
	}else {
		DeltaPosition->X += vr[0];

	}

	if (DeltaPosition->Z + vr[1] >= 198 || DeltaPosition->Z + vr[1] <= -198) {
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	else {

		DeltaPosition->Z += vr[1];
	}
	

	return result;
}

bool Scene::Shutdown() {
	bool result = true;

	if (Camera)
	{
		delete Camera;
		Camera = 0;
	}

	if (LightShader)
	{
		LightShader->Shutdown();
		delete LightShader;
		LightShader = 0;
	}

	if (Triangulo) {
		Triangulo->Shutdown();
		delete Triangulo;
		Triangulo = 0;
	}

	if (Object3d) {
		Object3d->Shutdown();
		delete Object3d;
		Object3d = 0;
	}

	return result;
}