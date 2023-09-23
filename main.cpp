#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"

#include <vector>
#include <string>
#define moren glm::vec3(1.0,1.0,1.0)
class MatrixStack {
	int		_index;
	int		_size;
	glm::mat4* _matrices;

public:
	MatrixStack(int numMatrices = 100) :_index(0), _size(numMatrices)
	{
		_matrices = new glm::mat4[numMatrices];
	}

	~MatrixStack()
	{
		delete[]_matrices;
	}

	void push(const glm::mat4& m) {
		assert(_index + 1 < _size);
		_matrices[_index++] = m;
	}

	glm::mat4& pop() {
		assert(_index - 1 >= 0);
		_index--;
		return _matrices[_index];
	}
};

int WIDTH = 600;
int HEIGHT = 600;

int mainWindow;
bool daytime = true;   // 判断是否为黑夜
bool playball = false; // 是否打球
bool jump = false; // 是否跳起来
bool KEY_C = false;

Camera* camera = new Camera();
Light* light = new Light();
MeshPainter* painter = new MeshPainter();
MeshPainter* painter3 = new MeshPainter();

// 这个用来回收和删除我们创建的物体对象
std::vector<TriMesh*> meshList;

TriMesh* boy_Torso = new TriMesh();
TriMesh* boy_Head = new TriMesh();
TriMesh* boy_LeftUpperArm = new TriMesh();
TriMesh* boy_RightLowerArm = new TriMesh();
TriMesh* boy_RightUpperArm = new TriMesh();
TriMesh* boy_LeftLowerArm = new TriMesh();
TriMesh* boy_UpperLeg = new TriMesh();
TriMesh* boy_LowerLeg = new TriMesh();

openGLObject TorsoObject;
openGLObject HeadObject;
openGLObject RightUpperArmObject;
openGLObject RightLowerArmObject;
openGLObject LeftUpperArmObject;
openGLObject LeftLowerArmObject;
openGLObject UpperLegObject;
openGLObject LowerLegObject;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

TriMesh* dreidel = new TriMesh();
TriMesh* basketball = new TriMesh();
TriMesh* Skybox_daytime = new TriMesh();
TriMesh* Skybox = new TriMesh();
TriMesh* sun = new TriMesh();
TriMesh* moon = new TriMesh();

void init()
{
	std::string vshader, fshader;
	// 读取着色器并使用
	vshader = "shaders/vshader.glsl";
	fshader = "shaders/fshader.glsl";

	// 设置光源位置
	light->setTranslation(glm::vec3(-6, 11.0, -8.0));
	light->setAmbient(glm::vec4(1.0, 1.0, 1.0 ,1.0)); // 环境光
	light->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	light->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	light->setAttenuation(1.0, 0.045, 0.0075); // 衰减系数
	//glEnable(GL_LIGHT0);      //允许0#灯使用
	//glEnable(GL_LIGHTING);   //开灯

	// 狗
	TriMesh* dog = new TriMesh();
	dog->setNormalize(true);
	dog->readObj("./assets/Dog/dog.obj");
	// 设置物体的旋转位移
	dog->setTranslation(glm::vec3(9.0, 0.7, 4.5));
	dog->setRotation(glm::vec3(0.0, -20.0, 0.0));
	dog->setScale(glm::vec3(2.5, 2.5, 2.5));
	dog->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	dog->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	dog->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	dog->setShininess(1.0); //高光系数
	// 加到painter中
	painter->addMesh(dog, "mesh_dog", "./assets/Dog/dog.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(dog);

	// 树
	TriMesh* tree = new TriMesh();
	tree->setNormalize(true);
	tree->readObj("./assets/Tree/tree.obj");
	// 设置物体的旋转位移
	tree->setTranslation(glm::vec3(2.5, 1.9, -3.0));
	tree->setRotation(glm::vec3(0.0, 0.0, 0.0));
	tree->setScale(glm::vec3(7.0, 7.0, 7.0));
	tree->setAmbient(glm::vec4(0.5, 0.5, 0.5, 1.0)); // 环境光
	tree->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	tree->setSpecular(glm::vec4(0.5, 0.5, 0.5, 1.0)); // 镜面反射
	tree->setShininess(1.0); //高光系数
	// 加到painter中
	painter->addMesh(tree, "mesh_tree", "./assets/Tree/tree.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(tree);

	// 湖
	TriMesh* lake = new TriMesh();
	lake->setNormalize(true);
	lake->readObj("./assets/Lake/lake.obj");
	// 设置物体的旋转位移
	lake->setTranslation(glm::vec3(7.0, 0.1, -5.0));
	lake->setRotation(glm::vec3(0.0, 0.0, 0.0));
	lake->setScale(glm::vec3(7.0, 7.0, 7.0));
	lake->setAmbient(glm::vec4(1.0 ,1.0 ,1.0, 1.0)); // 环境光
	lake->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	lake->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	lake->setShininess(500.0); //高光系数
	// 加到painter中
	painter->addMesh(lake, "mesh_lake", "./assets/Lake/lake.png", vshader, fshader); 	// 指定纹理与着色器
	meshList.push_back(lake);

	// 太阳
	sun->setNormalize(true);
	sun->readObj("./assets/Sun/sun.obj");
	// 设置物体的旋转位移
	sun->setTranslation(glm::vec3(-6, 12, -15.0));
	sun->setRotation(glm::vec3(-90.0, 0.0, 0.0));
	sun->setScale(glm::vec3(10.0, 10.0, 10.0));
	sun->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	sun->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	sun->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	sun->setShininess(1000.0); //高光系数
	// 加到painter中
	painter->addMesh(sun, "mesh_sun", "./assets/Sun/sun.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(sun);

	// 月亮
	moon->setNormalize(true);
	moon->readObj("./assets/Moon/moon.obj");
	// 设置物体的旋转位移
	moon->setTranslation(glm::vec3(0.0,-100,0.0));
	moon->setRotation(glm::vec3(0.0, 0.0, 0.0));
	moon->setScale(glm::vec3(10,10, 10));
	moon->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	moon->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	moon->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	moon->setShininess(1.0); //高光系数
	// 加到painter中
	painter->addMesh(moon, "mesh_moon", "./assets/Moon/moon.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(moon);
	
	// 奖杯
	TriMesh* cup = new TriMesh();
	cup->setNormalize(true);
	cup->readObj("./assets/Cup/cup.obj");
	// 设置物体的旋转位移
	cup->setTranslation(glm::vec3(3.75, 2.3, 5.0));
	cup->setRotation(glm::vec3(0.0, 0.0, 0.0));
	cup->setScale(glm::vec3(1.0, 1.0, 1.0));
	cup->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	cup->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	cup->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	cup->setShininess(100.0); //高光系数
	// 加到painter中
	painter->addMesh(cup, "mesh_cup", "./assets/Cup/cup.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(cup);
	
	// 房子
	TriMesh* house = new TriMesh();
	house->setNormalize(true);
	house->readObj("./assets/House/house.obj");
	// 设置物体的旋转位移
	house->setTranslation(glm::vec3(-7.0, 3.3, -4.0));
	house->setRotation(glm::vec3(0.0, 0.0, 0.0));
	house->setScale(glm::vec3(15.0, 15.0, 15.0));
	house->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	house->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	house->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	house->setShininess(3.0); //高光系数
	// 加到painter中
	painter->addMesh(house, "mesh_house", "./assets/House/house.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(house);

	
	// 草地
	TriMesh* grass = new TriMesh();
	grass->setNormalize(true);
	grass->readObj("./assets/Grass/grass.obj");
	// 设置物体的旋转位移
	grass->setTranslation(glm::vec3(0.0, -0.02, 0.0));
	grass->setRotation(glm::vec3(90.0, 0.0, 0.0));
	grass->setScale(glm::vec3(100.0, 100.0, 1.0));
	grass->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	grass->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	grass->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	grass->setShininess(10.0); //高光系数
	// 加到painter中
	painter->addMesh(grass, "mesh_grass", "./assets/Grass/grass.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(grass);

	//TriMesh* dreidel = new TriMesh();
	dreidel->setNormalize(true);
	dreidel->readObj("./assets/Dreidel/dreidel.obj");
	// 设置物体的旋转位移
	dreidel->setTranslation(glm::vec3(5.0, 0.85, 3.0));
	dreidel->setRotation(glm::vec3(90.0, 0.0, 0.0));
	dreidel->setScale(glm::vec3(2.0,2.0,2.0));
	dreidel->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	dreidel->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	dreidel->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	dreidel->setShininess(100.0); //高光系数
	// 加到painter中
	painter->addMesh(dreidel, "mesh_dreidel", "./assets/Dreidel/dreidel.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(dreidel);
	
	
	// 飞机
	TriMesh* plane = new TriMesh();
	plane->setNormalize(true);
	plane->readObj("./assets/Plane/plane.obj");
	// 设置物体的旋转位移
	plane->setTranslation(glm::vec3(-1.0, 8.0, -15.0));
	plane->setRotation(glm::vec3(-90.0, 0.0, 45.0));
	plane->setScale(glm::vec3(8.0, 8.0, 8.0));
	plane->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	plane->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	plane->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	plane->setShininess(200.0); //高光系数
	// 加到painter中
	painter->addMesh(plane, "mesh_plane", "./assets/Plane/plane.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(plane);

	// 篮球
	basketball->setNormalize(true);
	basketball->readObj("./assets/Basketball/basketball.obj");
	// 设置物体的旋转位移
	basketball->setTranslation(glm::vec3(1.2, 1.5, 4.9));
	basketball->setRotation(glm::vec3(0.0, 0.0, 0.0));
	basketball->setScale(glm::vec3(1.0, 1.0, 1.0));
	plane->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	plane->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	plane->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	plane->setShininess(3.0); //高光系数
	// 加到painter中
	painter->addMesh(basketball, "mesh_basketball", "./assets/Basketball/basketball.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(basketball);


	// 人物
	// 身体
	boy_Torso->setNormalize(true);
	boy_Torso->readObj("./assets/boy_kun/kun-7.obj");
	painter3->bindObjectAndData(boy_Torso, TorsoObject, "./assets/boy_kun/kun-7.png", vshader, fshader);
	boy_Torso->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	boy_Torso->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	boy_Torso->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	meshList.push_back(boy_Torso);//程序结束后释放数据
	// 设置物体的旋转位移
	boy_Torso->setTranslation(glm::vec3(2.5, 1.6, 5));
	boy_Torso->setRotation(glm::vec3(0.0, 0.0, 0.0));
	boy_Torso->setScale(glm::vec3(1.0, 1.0, 1.0));
	boy_Torso->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	boy_Torso->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	boy_Torso->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	// 加到painter中
	painter->addMesh(boy_Torso, "mesh_cup", "./assets/boy_kun/kun-7.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(boy_Torso);
	
	// 头
	boy_Head->setNormalize(true);
	boy_Head->readObj("./assets/boy_kun/kun-6.obj");
	painter3->bindObjectAndData(boy_Head, HeadObject, "./assets/boy_kun/kun-6.png", vshader, fshader);
	meshList.push_back(boy_Head);//程序结束后释放数据
	// 设置物体的旋转位移
	boy_Head->setTranslation(glm::vec3(2.5, 2.13, 5));
	boy_Head->setRotation(glm::vec3(0.0, 0.0, 0.0));
	boy_Head->setScale(glm::vec3(1.0, 1.0, 1.0));
	boy_Head->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	boy_Head->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	boy_Head->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	// 加到painter中
	painter->addMesh(boy_Head, "mesh_cup", "./assets/boy_kun/kun-6.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(boy_Head);
	

	// 右上臂
	boy_RightUpperArm->setNormalize(true);
	boy_RightUpperArm->readObj("./assets/boy_kun/kun-2.obj");

	painter3->bindObjectAndData(boy_RightUpperArm, LeftUpperArmObject, "./assets/boy_kun/kun-2.png", vshader, fshader);
	boy_RightUpperArm->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	boy_RightUpperArm->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	boy_RightUpperArm->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	meshList.push_back(boy_RightUpperArm);//程序结束后释放数据
	// 设置物体的旋转位移
	boy_RightUpperArm->setTranslation(glm::vec3(1.75, 1.9, 5.0));
	boy_RightUpperArm->setRotation(glm::vec3(0.0, 0.0, 0.0));
	boy_RightUpperArm->setScale(glm::vec3(1.0, 1.0, 1.0));
	// 加到painter中
	painter->addMesh(boy_RightUpperArm, "mesh_cup", "./assets/boy_kun/kun-2.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(boy_RightUpperArm);
	
	// 右下臂
	boy_RightLowerArm->setNormalize(true);
	boy_RightLowerArm->readObj("./assets/boy_kun/kun-3.obj");
	painter3->bindObjectAndData(boy_RightLowerArm, RightLowerArmObject, "./assets/boy_kun/kun-3.png", vshader, fshader);
	boy_RightLowerArm->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	boy_RightLowerArm->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	boy_RightLowerArm->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	meshList.push_back(boy_RightLowerArm);//程序结束后释放数据
	
	// 设置物体的旋转位移
	boy_RightLowerArm->setTranslation(glm::vec3(1.55, 1.83, 5));
	boy_RightLowerArm->setRotation(glm::vec3(0.0, 0.0, 0.0));
	boy_RightLowerArm->setScale(glm::vec3(1.0, 1.0, 1.0));
	// 加到painter中
	painter->addMesh(boy_RightLowerArm, "mesh_cup", "./assets/boy_kun/kun-3.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(boy_RightLowerArm);
	

	// 左上臂
	boy_LeftUpperArm->setNormalize(true);
	boy_LeftUpperArm->readObj("./assets/boy_kun/kun-4.obj");
	painter3->bindObjectAndData(boy_LeftUpperArm, RightUpperArmObject, "./assets/boy_kun/kun-4.png", vshader, fshader);
	boy_LeftUpperArm->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	boy_LeftUpperArm->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	boy_LeftUpperArm->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	meshList.push_back(boy_LeftUpperArm);//程序结束后释放数据
	// 设置物体的旋转位移
	boy_LeftUpperArm->setTranslation(glm::vec3(3.25 , 1.90 ,5));
	boy_LeftUpperArm->setRotation(glm::vec3(0.0, 0.0, 0.0));
	boy_LeftUpperArm->setScale(glm::vec3(1.0, 1.0, 1.0));

	// 加到painter中
	painter->addMesh(boy_LeftUpperArm, "mesh_cup", "./assets/boy_kun/kun-4.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(boy_LeftUpperArm);
	
	//左下臂
	boy_LeftLowerArm->setNormalize(true);
	boy_LeftLowerArm->readObj("./assets/boy_kun/kun-5.obj");
	painter3->bindObjectAndData(boy_LeftLowerArm, LeftLowerArmObject, "./assets/boy_kun/kun-5.png", vshader, fshader);
	boy_LeftLowerArm->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	boy_LeftLowerArm->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	meshList.push_back(boy_LeftLowerArm);//程序结束后释放数据
	
	// 设置物体的旋转位移
	boy_LeftLowerArm->setTranslation(glm::vec3(3.45, 1.83, 5));
	boy_LeftLowerArm->setRotation(glm::vec3(0.0, 0.0, 0.0));
	boy_LeftLowerArm->setScale(glm::vec3(1.0, 1.0, 1.0));
	// 加到painter中
	painter->addMesh(boy_LeftLowerArm, "mesh_cup", "./assets/boy_kun/kun-5.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(boy_LeftUpperArm);

	// 大腿
	boy_UpperLeg->setNormalize(true);
	boy_UpperLeg->readObj("./assets/boy_kun/kun-0.obj");
	painter3->bindObjectAndData(boy_UpperLeg,UpperLegObject, "./assets/boy_kun/kun-0.png", vshader, fshader);
	boy_UpperLeg->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	boy_UpperLeg->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	boy_UpperLeg->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	meshList.push_back(boy_UpperLeg);//程序结束后释放数据
	// 设置物体的旋转位移
	boy_UpperLeg->setTranslation(glm::vec3(2.5, 1.0, 5));
	boy_UpperLeg->setRotation(glm::vec3(0.0, 0.0, 0.0));
	boy_UpperLeg->setScale(glm::vec3(1.0, 1.0, 1.0));
	// 加到painter中
	painter->addMesh(boy_UpperLeg, "mesh_cup", "./assets/boy_kun/kun-0.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(boy_UpperLeg);
	
	//小腿
	boy_LowerLeg->setNormalize(true);
	boy_LowerLeg->readObj("./assets/boy_kun/kun-1.obj");
	painter3->bindObjectAndData(boy_LowerLeg, LowerLegObject, "./assets/boy_kun/kun-1.png", vshader, fshader);
	boy_LowerLeg->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	boy_LowerLeg->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	boy_LowerLeg->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	meshList.push_back(boy_LowerLeg);//程序结束后释放数据
	// 设置物体的旋转位移
	boy_LowerLeg->setTranslation(glm::vec3(2.5, 0.35,5.1));
	boy_LowerLeg->setRotation(glm::vec3(0.0, 0.0, 0.0));
	boy_LowerLeg->setScale(glm::vec3(1.0, 1.0, 1.0));
	// 加到painter中
	painter->addMesh(boy_LowerLeg, "mesh_cup", "./assets/boy_kun/kun-1.png", vshader, fshader); 	// 指定纹理与着色器
	// 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
	meshList.push_back(boy_LowerLeg);

	// 天空盒
	Skybox_daytime->setNormalize(true);
	Skybox_daytime->readObj("./assets/skybox/skybox.obj");
	// 设置物体的旋转位移
	Skybox_daytime->setTranslation(glm::vec3(0.0, 0, 0.0));
	Skybox_daytime->setRotation(glm::vec3(0.0, 0.0, 0.0));
	Skybox_daytime->setScale(glm::vec3(200, 200, 200));
	Skybox_daytime->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	Skybox_daytime->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	Skybox_daytime->setSpecular(glm::vec4(0.9, 0.9, 0.9, 1.0)); // 镜面反射
	Skybox_daytime->setShininess(10.0); //高光系数
	// 加到painter中
	painter->addMesh(Skybox_daytime, "mesh_a", "./assets/skybox/skybox2.png", vshader, fshader);
	meshList.push_back(Skybox_daytime);
	
	Skybox->setNormalize(true);
	Skybox->readObj("./assets/skybox/skybox.obj");
	// 设置物体的旋转位移
	Skybox->setTranslation(glm::vec3(0.0, 0, 0.0));
	Skybox->setRotation(glm::vec3(0.0, 90.0, 0.0));
	Skybox->setScale(glm::vec3(210, 210, 210));
	Skybox->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	Skybox->setDiffuse(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 漫反射
	Skybox->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	Skybox->setShininess(1.0); //高光系数
		// 加到painter中
	painter->addMesh(Skybox, "mesh_a", "./assets/skybox/skybox3.png", vshader, fshader);
	
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClearColor(0.0, 0.0, 0.0, 1.0);
}

int start = 0;
int speedrotate = 5;
void autorotate() {
	start++;
	if (start > 4) {
		glm::vec3 a = dreidel->getRotation();
		a.y += speedrotate;
		dreidel->setRotation(glm::vec3{ a.x,a.y,a.z });
		start = 0;
	}
}

int start1 = 0;
float ballSpeed = 0.1; // 篮球上下移动速度
int flag = 0;// 0下降的时候, 1上升的时候
int kflag = 1; // 控制抬手和下拍手的时机
void playBasketball()
{	
	start1++;
	if (start1 > 4 && flag == 0) {
		start1 = 0;
		glm::vec3 tran = basketball->getTranslation();
		if (tran.y < 1.3 && kflag) // 球下降往下拍手
			{
				kflag = 0;
				glm::vec3 tempR1 = boy_RightUpperArm->getRotation();
				glm::vec3 tempR2 = boy_RightLowerArm->getRotation();
				glm::vec3 tempT1 = boy_RightUpperArm->getTranslation();
				glm::vec3 tempT2 = boy_RightLowerArm->getTranslation();
				tempR1.z += 20;
				tempR2.z += 20;
				tempT1.y -= 0.2;
				tempT2.y -= 0.2;
				boy_RightUpperArm->setRotation(tempR1);
				boy_RightLowerArm->setRotation(tempR2);
				boy_RightUpperArm->setTranslation(tempT1);
				boy_RightLowerArm->setTranslation(tempT2);
			}
		
		if (tran.y > 0.3)
			tran.y -= ballSpeed;
		else {
			flag = 1;
		}
		basketball->setTranslation(tran);
	}
	else if (start1 > 4 && flag == 1) {
		start1 = 0;
		glm::vec3 tran = basketball->getTranslation();
		if (tran.y < 1.5)
		{
			tran.y += ballSpeed;
			if (tran.y > 1.2 && kflag == 0) // 球上升往上抬手
			{
				kflag = 1;
				glm::vec3 tempR1 = boy_RightUpperArm->getRotation();
				glm::vec3 tempR2 = boy_RightLowerArm->getRotation();
				glm::vec3 tempT1 = boy_RightUpperArm->getTranslation();
				glm::vec3 tempT2 = boy_RightLowerArm->getTranslation();
				tempR1.z -= 20;
				tempR2.z -= 20;
				tempT1.y += 0.2;
				tempT2.y += 0.2;
				boy_RightUpperArm->setRotation(tempR1);
				boy_RightLowerArm->setRotation(tempR2);
				boy_RightUpperArm->setTranslation(tempT1);
				boy_RightLowerArm->setTranslation(tempT2);
			}
		}
		else {
			flag = 0;
		}
		basketball->setTranslation(tran);
	}
}

void display()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera->updateCamera();
	camera->viewMatrix = camera->getViewMatrix();
	camera->projMatrix = camera->getProjectionMatrix(false);

	// 陀螺不断旋转
	autorotate();
	// 是否拍球
	if (playball)
	{
		playBasketball();
	}
	painter->drawMeshes(light, camera);
	
}


void printHelp()
{
	std::cout << "================================================" << std::endl;
	std::cout << "Use mouse to controll the light position (drag)." << std::endl;
	std::cout << "================================================" << std::endl << std::endl;

	std::cout << "Keyboard Usage" << std::endl;
	std::cout <<
		"[Window]" << std::endl <<
		"ESC:		Exit" << std::endl <<
		"h:		Print help message" << std::endl <<

	std::endl <<
		"[Camera]" << std::endl <<
		"SPACE:		Reset camera parameters" << std::endl <<
		"u(U)/u(U)+shift:		Increase/Decrease the rotate angle" << std::endl <<
		"i(I)/i(I)+shift:		Increase/Decrease the up angle" << std::endl <<
		"o(O)/o(O)+shift:		Increase/Decrease the camera radius" << std::endl << 
		"c(C):		observe the scene in an orthogonal or perspective projection" << std::endl << std::endl;
	std::cout <<
		"[Light]" << std::endl <<
		"c(C):		Switch day and night" << std::endl <<
		"x / (shift + x):	move the light along X positive / negative axis" << std::endl <<
		"y/(shift+y):		move the light along Y positive/negative axis" << std::endl <<
		"v(V):		observe the scene in an orthogonal or perspective projection" << std::endl <<
		"z/(shift+z):		move the light along Z positive/negative axis" << std::endl << std::endl;

	std::cout <<
		"[model]" << std::endl <<
		"p/P:			Boy start/stop playing basketball" << std::endl <<
		"t/T:			Boy start/stop jumping" << std::endl <<
		"up/down:		Increase/Decrease the rotate speend of the dreidel" << std::endl <<
		"a(A)/q(Q):		Increase/Decrease the basketball movement speed" << std::endl << std::endl;
	
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float tmp;
	glm::vec3 temp;
	glm::vec4 ambient;
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_ESCAPE: exit(EXIT_SUCCESS); break;
		case GLFW_KEY_H: printHelp(); break;

		case GLFW_KEY_C:  // 白天黑夜的改变
		{	
			if (daytime)
			{
				moon->setTranslation(glm::vec3(6, 12, -15.0));
				sun->setTranslation(glm::vec3(0, -100, 0.0));
				// 设置光源位置
				light->setTranslation(glm::vec3(6.0, 10.0, -8.0));
				light->setAmbient(glm::vec4(0.01,0.01,0.01,1)); // 环境光
				light->setDiffuse(glm::vec4(0.01, 0.01, 0.01, 1)); // 漫反射
				light->setSpecular(glm::vec4(0.01, 0.01, 0.01, 1)); // 镜面反射
				light->setAttenuation(1.0, 0.07, 0.009); // 衰减系数
				Skybox_daytime->setScale(glm::vec3(220, 220, 220));
			}
			else
			{
				moon->setTranslation(glm::vec3(0, -100, 0.0));
				sun->setTranslation(glm::vec3(-6, 12, -15.0));
				// 设置光源位置
				light->setTranslation(glm::vec3(-6, 11.0, -8.0));
				light->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
				light->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
				light->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
				light->setAttenuation(1.0, 0.045, 0.0075); // 衰减系数
				Skybox_daytime->setScale(glm::vec3(200, 200, 200));
			}
			daytime = !daytime;
		}
		break;
		// 让光源位置沿xyz轴变化
		case GLFW_KEY_X: 
		{
			if(mode == GLFW_MOD_SHIFT)
			{
				temp = light->getTranslation();
				temp.x -= 10;
				light->setTranslation(temp);
			}
			else if (mode == 0x0000)
			{
				temp = light->getTranslation();
				temp.x += 10;
				light->setTranslation(temp);
			}
		}
		break;
		case GLFW_KEY_Y:
		{
			if (mode == GLFW_MOD_SHIFT)
			{
				temp = light->getTranslation();
				temp.y -= 10;
				light->setTranslation(temp);
			}
			else if (mode == 0x0000)
			{
				temp = light->getTranslation();
				temp.y += 10;
				light->setTranslation(temp);
			}
		}
		break;
		case GLFW_KEY_Z:
		{
			if (mode == GLFW_MOD_SHIFT)
			{
				temp = light->getTranslation();
				temp.z -= 10;
				light->setTranslation(temp);
			}
			else if (mode == 0x0000)
			{
				temp = light->getTranslation();
				temp.z += 10;
				light->setTranslation(temp);
			}
		}
		break;
		// 增减篮球上下移动速度
		case GLFW_KEY_A: if(ballSpeed<0.3) ballSpeed += 0.03; break;  // 有速度阈值
		case GLFW_KEY_Q: if(ballSpeed>0.3) ballSpeed -= 0.03; break;
		
		// 增减陀螺旋转速度
		case GLFW_KEY_UP: speedrotate += 2; break;
		case GLFW_KEY_DOWN: speedrotate -= 2; break;

		case GLFW_KEY_P: playball = !playball; break;
		case GLFW_KEY_J:  // boy跳起动作
		{
			if (!jump)
			{
				jump = !jump;
				glm::vec3 tempR1 = boy_UpperLeg->getRotation();
				glm::vec3 tempR2 = boy_LowerLeg->getRotation();
				glm::vec3 tempT1 = boy_UpperLeg->getTranslation();
				glm::vec3 tempT2 = boy_LowerLeg->getTranslation();
				tempR1.x -= 50;
				tempT1.y += 0.2;
				tempT1.z += 0.3;
				tempR2.x += 40;
				tempT2.z += 0.4;
				tempT2.y += 0.35;
				
				boy_UpperLeg->setRotation(tempR1);
				boy_UpperLeg->setTranslation(tempT1);
				boy_LowerLeg->setRotation(tempR2);
				boy_LowerLeg->setTranslation(tempT2);
			}
			else
			{
				jump = !jump;
				glm::vec3 tempR1 = boy_UpperLeg->getRotation();
				glm::vec3 tempR2 = boy_LowerLeg->getRotation();
				glm::vec3 tempT1 = boy_UpperLeg->getTranslation();
				glm::vec3 tempT2 = boy_LowerLeg->getTranslation();
				tempR1.x += 50;
				tempT1.y -= 0.2;
				tempT1.z -= 0.3;
				tempR2.x -= 40;
				tempT2.z -= 0.4;
				tempT2.y -= 0.35;

				boy_UpperLeg->setRotation(tempR1);
				boy_UpperLeg->setTranslation(tempT1);
				boy_LowerLeg->setRotation(tempR2);
				boy_LowerLeg->setTranslation(tempT2);
			}
			
		}break;
		default:
			camera->keyboard(key, action, mode);
			break;
		}
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera->mouse_callback(window, xpos, ypos);
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->scroll_callback(window, xoffset, yoffset);
}
void cleanData() {
	delete camera;
	camera = NULL;

	delete light;
	light = NULL;

	painter->cleanMeshes();

	delete painter;
	painter = NULL;

	for (int i = 0; i < meshList.size(); i++) {
		delete meshList[i];
	}
	meshList.clear();
}


int main(int argc, char** argv)
{
	// 初始化GLFW库，必须是应用程序调用的第一个GLFW函数
	glfwInit();

	// 配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 配置窗口属性
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "2020152114_黄亚中_期末大作业", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Init mesh, shaders, buffer
	init();
	// 输出帮助信息
	printHelp();
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	cleanData();

	return 0;
}