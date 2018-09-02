
#include <graphics/camera.h>

namespace graphics {

Camera::Camera(vmath::vec3  pos, float phi, float theta, float speed)
	: m_pos(pos), m_speed(speed), m_phi(phi), m_theta(theta)
{
	m_front = calculateFront();
	m_right = -vmath::cross(vmath::vec3(0.0, 1.0f, 0.0), m_front);
	m_up = vmath::cross(m_front, m_right);
}

Camera::Camera(vmath::vec3 eye, vmath::vec3  center, vmath::vec3 up) {

}

void
Camera::move(vmath::vec3 direction)
{
	m_pos += m_speed * direction;
}

vmath::mat4 Camera::getMatirxMatrix()
{
	vmath::vec3 worldup(0.0f, 1.0f, 0.0f);
	vmath::vec3 front = calculateFront();
	return lookat(m_pos, m_pos + front, worldup);
}

vmath::vec3 Camera::getfront()
{
	m_front=calculateFront();
	return m_front;
}

vmath::vec3 Camera::getup()
{
	return m_up;
}

vmath::vec3 Camera::getright()
{
	getfront();
	return -cross(vmath::vec3(0.0f, 1.0f, 0.0f), m_front);
}

vmath::vec3 Camera::getPosition()
{
	return m_pos;
}

void Camera::point(float pX, float pY)
{
	static float old_pX = 0;
	static float old_pY = 0;
	int angular_velocity = 1;
	float Nphi = m_phi + angular_velocity * (pX - old_pX > 0 ? 0.25f : -0.25f);
	if (Nphi > 180.0f)
		Nphi -= 180.0f;
	else if (Nphi < -180.0f)
		Nphi += 180.0f;
	else
		m_phi = Nphi;
	float Ntheta = m_theta + angular_velocity * (pY - old_pY > 0 ? -0.25f : 0.25f);
	if (Ntheta > -80 && Ntheta < 80)
		m_theta = Ntheta;
	old_pX = pX;
	old_pY = pY;
}

vmath::vec3 Camera::calculateFront() {

	return normalize(vmath::vec3(sin(vmath::radians(m_phi)), sin(vmath::radians(m_theta)), -cos(vmath::radians(m_phi))));
}

void Camera::set_instirc_property(float fovy, float aspect, float n ,float f) {
	this->instric_matrix = vmath::perspective(fovy, aspect, n, f);
}

vmath::mat4 Camera::pipeline() {
	return instric_matrix * getMatirxMatrix();
}

}
