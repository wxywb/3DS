#include<graphics/common.h>

#ifndef CAMERA_H
#define CAMERA_H
namespace graphics {

class Camera
{
public:
	Camera(vmath::vec3  pos, float phi, float theta, float speed);

	Camera(vmath::vec3  eye, vmath::vec3  center, vmath::vec3  up);

	void move(vmath::vec3 direction);

	vmath::mat4 getMatirxMatrix();

	vmath::vec3 getfront();

	vmath::vec3 getup();

	vmath::vec3  getright();

	vmath::vec3  getPosition();

	inline vmath::vec3  calculateFront();

	void point(float pX, float pY);

	void set_instirc_property(float fovy, float aspect, float n, float f);

	vmath::mat4 pipeline();

private:
	vmath::vec3   m_up;
	vmath::vec3   m_front;
	vmath::vec3   m_right;
	vmath::vec3   m_pos;
	float		  m_theta;
	float		  m_phi;
	float		  m_speed;
	vmath::mat4	  instric_matrix;
};

}
#endif#include<graphics/common.h>

#ifndef CAMERA_H
#define CAMERA_H
namespace graphics {

class Camera
{
public:
	Camera(vmath::vec3  pos, float phi, float theta, float speed);

	Camera(vmath::vec3  eye, vmath::vec3  center, vmath::vec3  up);

	void move(vmath::vec3 direction);

	vmath::mat4 getMatirxMatrix();

	vmath::vec3 getfront();

	vmath::vec3 getup();

	vmath::vec3  getright();

	vmath::vec3  getPosition();

	inline vmath::vec3  calculateFront();

	void point(float pX, float pY);

	void set_instirc_property(float fovy, float aspect, float n, float f);

	vmath::mat4 pipeline();

private:
	vmath::vec3   m_up;
	vmath::vec3   m_front;
	vmath::vec3   m_right;
	vmath::vec3   m_pos;
	float		  m_theta;
	float		  m_phi;
	float		  m_speed;
	vmath::mat4	  instric_matrix;
};

}
#endif