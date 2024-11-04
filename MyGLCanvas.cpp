#define NUM_OPENGL_LIGHTS 8

#include "MyGLCanvas.h"
// #include "Camera.h"

int Shape::m_segmentsX;
int Shape::m_segmentsY;

MyGLCanvas::MyGLCanvas(int x, int y, int w, int h, const char *l) : Fl_Gl_Window(x, y, w, h, l) {
	mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
	
	rotVec = glm::vec3(0.0f, 0.0f, 0.0f);
	eyePosition = glm::vec3(2.0f, 2.0f, 2.0f);

	pixelWidth = w;
	pixelHeight = h;

	isectOnly = 1;
	segmentsX = segmentsY = 10;
	scale = 1.0f;
	parser = NULL;

	objType = SHAPE_CUBE;
	cube = new Cube();
	cylinder = new Cylinder();
	cone = new Cone();
	sphere = new Sphere();
	shape = cube;

	shape->setSegments(segmentsX, segmentsY);

	camera = new Camera();
	camera->orientLookAt(eyePosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

MyGLCanvas::~MyGLCanvas() {
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	if (camera != NULL) {
		delete camera;
	}
	if (parser != NULL) {
		delete parser;
	}
	if (pixels != NULL) {
		delete pixels;
	}
}

void MyGLCanvas::renderShape(OBJ_TYPE type) {
	objType = type;
	switch (type) {
	case SHAPE_CUBE:
		shape = cube;
		break;
	case SHAPE_CYLINDER:
		shape = cylinder;
		break;
	case SHAPE_CONE:
		shape = cone;
		break;
	case SHAPE_SPHERE:
		shape = sphere;
		break;
	case SHAPE_SPECIAL1:
	default:
		shape = cube;
	}

	// shape->setSegments(segmentsX, segmentsY);
	// shape->draw();
}

void MyGLCanvas::loadSceneFile(const char* filenamePath) {
    if (parser != NULL) {
        delete parser;
        delete scene;
    }
    parser = new SceneParser(filenamePath);

    bool success = parser->parse();
    cout << "success? " << success << endl;
    if (success == false) {
        delete parser;
        delete scene;
        parser = NULL;
        scene = NULL;
    }
    else {
        SceneCameraData cameraData;
        parser->getCameraData(cameraData);

        camera->reset();
        camera->setViewAngle(cameraData.heightAngle);
        if (cameraData.isDir == true) {
            camera->orientLookVec(cameraData.pos, cameraData.look, cameraData.up);
        }
        else {
            camera->orientLookAt(cameraData.pos, cameraData.lookAt, cameraData.up);
        }
        this->scene = new SceneGraph();
        flatSceneData();
    }
}

void MyGLCanvas::flatSceneData() {
    flatSceneDataRec(parser->getRootNode(), glm::mat4(1.0f));
    // this->scene->calculate();ß
}

void MyGLCanvas::flatSceneDataRec(SceneNode* node, glm::mat4 curMat) {
    for (SceneTransformation* transform : node->transformations) {
        switch (transform->type) {
            case TRANSFORMATION_SCALE:
                curMat = glm::scale(curMat, transform->scale);
                break;
            case TRANSFORMATION_ROTATE:
                curMat = glm::rotate(curMat, transform->angle, transform->rotate);
                break;
            case TRANSFORMATION_TRANSLATE:
                curMat = glm::translate(curMat, transform->translate);
                break;
            case TRANSFORMATION_MATRIX:
                curMat = curMat * transform->matrix;
                break;
        }
    }
    for (ScenePrimitive* primitive : node->primitives){
        switch (primitive->type) {
            case SHAPE_CUBE:
                this->scene->addNode(new SceneGraphNode(curMat, new Cube(), primitive->material));
                break;
            case SHAPE_CYLINDER:
                this->scene->addNode(new SceneGraphNode(curMat, new Cylinder(), primitive->material));
                break;
            case SHAPE_CONE:
                this->scene->addNode(new SceneGraphNode(curMat, new Cone(), primitive->material));
                break;
            case SHAPE_SPHERE:
                this->scene->addNode(new SceneGraphNode(curMat, new Sphere(), primitive->material));
                break;
            // case SHAPE_SPECIAL1:
            //     this->scene->addNode(new SceneGraphNode(curMat, new Special1(), primitive->material));
                break;
            default:
                this->scene->addNode(new SceneGraphNode(curMat, new Cube(), primitive->material));
        }
    }
    if (node->children.size() == 0) {
        return;
    }
    else {
        for (SceneNode* child : node->children){
            flatSceneDataRec(child, curMat);
        }
    }
}


void MyGLCanvas::setSegments() {
	shape->setSegments(segmentsX, segmentsY);
}

void MyGLCanvas::draw() {
	if (!valid()) {  //this is called when the GL canvas is set up for the first time or when it is resized...
		printf("establishing GL context\n");

		glViewport(0, 0, w(), h());
		updateCamera(w(), h());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	if (pixels == NULL) {
		return;
	}

	//this just draws the "pixels" to the screen
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(pixelWidth, pixelHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

int MyGLCanvas::handle(int e) {
	//printf("Event was %s (%d)\n", fl_eventnames[e], e);
	switch (e) {
	case FL_KEYUP:
		printf("keyboard event: key pressed: %c\n", Fl::event_key());
		break;
	case FL_MOUSEWHEEL:
		break;
	}

	return Fl_Gl_Window::handle(e);
}

void MyGLCanvas::resize(int x, int y, int w, int h) {
	Fl_Gl_Window::resize(x, y, w, h);
	if (camera != NULL) {
		camera->setScreenSize(w, h);
	}
	puts("resize called");
}


void MyGLCanvas::updateCamera(int width, int height) {
	float xy_aspect;
	xy_aspect = (float)width / (float)height;

	camera->setScreenSize(width, height);
}

//Given the pixel (x, y) position, set its color to (r, g, b)
void MyGLCanvas::setpixel(GLubyte* buf, int x, int y, int r, int g, int b) {
	pixelWidth = camera->getScreenWidth();
	buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
	buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
	buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

glm::vec3 MyGLCanvas::calculateIllumination(glm::vec3 isectPoint, glm::vec3 normal_object, SceneGraphNode* closestNode){
	glm::mat4 inverseViewMatrix = glm::inverse(closestNode->getTransformation());
	glm::vec3 normal = glm::normalize(glm::vec3(inverseViewMatrix * glm::vec4(normal_object, 0.0f)));
	glm::vec3 color(0.0f);  // Initialize final color (R, G, B)
    
    // Ambient component
    color.r += parser->getGlobalData().ka * closestNode->getMaterial().cAmbient.r;
    color.g += parser->getGlobalData().ka * closestNode->getMaterial().cAmbient.g;
    color.b += parser->getGlobalData().ka * closestNode->getMaterial().cAmbient.b;

	SceneLightData light;
	for (int i = 0; i < parser->getNumLights(); i++) {
		parser->getLightData(i, light);
		glm::vec3 lightDir;
        
		if (light.type == LightType::LIGHT_POINT) {
			lightDir = glm::normalize(light.pos - isectPoint);
		}
		else {
			lightDir = glm::normalize(-light.dir);
		}
		// Diffuse component
		float diffIntensity = glm::max(glm::dot(normal, lightDir), 0.0f);
		color.r += diffIntensity * light.color.r * closestNode->getMaterial().cDiffuse.r * parser->getGlobalData().kd;
		color.g += diffIntensity * light.color.g * closestNode->getMaterial().cDiffuse.g * parser->getGlobalData().kd;
		color.b += diffIntensity * light.color.b * closestNode->getMaterial().cDiffuse.b * parser->getGlobalData().kd;

		// Specular component
		glm::vec3 reflectDir = glm::normalize(glm::reflect(-lightDir, normal));
        float specIntensity = glm::pow(glm::max(glm::dot(glm::normalize(camera->getLookVector()), reflectDir), 0.0f), closestNode->getMaterial().shininess);
		color.r += specIntensity * light.color.r * closestNode->getMaterial().cSpecular.r * parser->getGlobalData().ks;
		color.g += specIntensity * light.color.g * closestNode->getMaterial().cSpecular.g * parser->getGlobalData().ks;
		color.b += specIntensity * light.color.b * closestNode->getMaterial().cSpecular.b * parser->getGlobalData().ks;
	}
	color.r = glm::clamp(color.r, 0.0f, 1.0f);
    color.g = glm::clamp(color.g, 0.0f, 1.0f);
    color.b = glm::clamp(color.b, 0.0f, 1.0f);
	return color;
}

void MyGLCanvas::raycasting(glm::vec3 eyePosition_world, glm::vec3 lookVector_world, int i, int j){
	// Iterate through all the object in the scene graph
	double mint = 1000000000;
	Shape* closestShape = NULL;
	SceneGraphNode* closestNode = NULL;

	for (auto it = this->scene->getIterator(); it != this->scene->getEnd(); ++it) {
		SceneGraphNode* node = *it;
		Shape* shape = node->getShape();
		// if(dynamic_cast<Sphere*>(shape) != nullptr){
		
		double t = shape->intersect(eyePosition_world, lookVector_world, node->getTransformation());
		if (t > 0 && t < mint) {
			mint = t;
			closestShape = shape;
			closestNode = node;
		// }
		}
	}
	// Have the intersection
	if (closestShape != NULL) {
		cout << "closest shape: " << closestShape->getType() << endl;
		// Compute the intersection point in world coordinates
		glm::vec3 isectPoint = eyePosition_world + (float)mint * lookVector_world;
		cout << "intersection point: " << isectPoint.x << ", " << isectPoint.y << ", " << isectPoint.z << endl;
		if(isectOnly == 1){
			// Set the pixel to white
			setpixel(pixels, i, j, 255, 255, 255);
		}
		else{
			// Get the intersection point in object space
			glm::mat4 inverseViewMatrix = glm::inverse(closestNode->getTransformation());
			glm::vec3 isectPoint_object = glm::vec3(inverseViewMatrix * glm::vec4(isectPoint, 1.0f));
			// Compute the normal at the intersection point and calculate the illumination
			glm::vec3 normal_object = closestShape->computeNormal(isectPoint_object);
			glm::vec3 normal_world = glm::vec3(closestNode->getTransformation() * glm::vec4(normal_object, 0.0f));
			glm::vec3 color = calculateIllumination(isectPoint, normal_world, closestNode);
			setpixel(pixels, i, j, color.r * 255, color.g * 255, color.b * 255);
		}
	}
	else{
		// Set the pixel to black
		setpixel(pixels, i, j, 0, 0, 0);
	}
}

glm::vec3 MyGLCanvas::getFilmPointWorld(int pixelX, int pixelY, int screenWidth, int screenHeight){
	float ndcX = -1.0f + (2.0f * pixelX) / (float)screenWidth;
    float ndcY = -1.0f + (2.0f * pixelY) / (float)screenHeight;  

    float tanHalfFovy = tan(camera->getViewAngle() * PI / 360.0f);
    float aspectRatio = (float)screenWidth / screenHeight;

    float nearZ = camera->getNearPlane();
    float x = ndcX * aspectRatio * tanHalfFovy * nearZ;
    float y = ndcY * tanHalfFovy * nearZ;
    float z = -nearZ;  

    glm::vec4 filmPoint_camera(x, y, z, 1.0f);
    glm::vec4 filmPoint_world = camera->getInverseModelViewMatrix() * filmPoint_camera;
    
    return glm::vec3(filmPoint_world) / filmPoint_world.w;
}

glm::vec3 MyGLCanvas::getRayDirectionInWorld(glm::vec3 filmPointInWorld, glm::vec3 eyePositionInWorld, glm::mat4 viewMatrix){
	// Get the ray direction in world coordinates with normalization
	return glm::normalize(filmPointInWorld - eyePositionInWorld);;
}

void MyGLCanvas::renderScene() {
	cout << "render button clicked!" << endl;

	if (parser == NULL) {
		cout << "no scene loaded yet" << endl;
		return;
	}

	pixelWidth = w();
	pixelHeight = h();


	updateCamera(pixelWidth, pixelHeight);

	if (pixels != NULL) {
		delete pixels;
	}
	pixels = new GLubyte[pixelWidth  * pixelHeight * 3];
	memset(pixels, 0, pixelWidth  * pixelHeight * 3);

	glm::vec3 eyePosition_world = camera->getEyePoint();
	for (int i = 0; i < pixelWidth; i++) {
		for (int j = 0; j < pixelHeight; j++) {
			//TODO: this is where your ray casting will happen!
			glm::vec3 filmPoint = getFilmPointWorld(i, j, pixelWidth, pixelHeight);
			// TODO: get the ray direction in world coordinates, need to use the view matrix
			glm::vec3 rayDirection_world = getRayDirectionInWorld(filmPoint, eyePosition_world, camera->getModelViewMatrix());
			raycasting(eyePosition_world, rayDirection_world, i, j);
			// if (isectOnly == 1) {
			// 	setpixel(pixels, i, j, 125, 125, 125);
			// }
			// else {
			// 	setpixel(pixels, i, j, 255, 255, 255);
			// }
		}
	}
	cout << "render complete" << endl;
	redraw();
}
