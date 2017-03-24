#pragma once

#include "SharedDefine.h"

class SimpleCamera
{
public:

	SimpleCamera()
	{
	}

	~SimpleCamera()
	{
	}

	glm::vec3 GetPosition() const
	{
		return position;
	}

	void SetPosition(float _x, float _y, float _z)
	{
		position = glm::vec3(_x, _y, _z);
	}

	void SetPosition(const glm::vec3 _newPos)
	{
		position = _newPos;
	}

	glm::vec3 GetRight() const
	{
		return right;
	}

	glm::vec3 GetUp() const
	{
		return up;
	}

	glm::vec3 GetLook() const
	{
		return look;
	}

	//Frustum Properties Getter
	float GetNearZ() const
	{
		return nearZ;
	}

	float GetFarZ() const
	{
		return farZ;
	}

	float GetAspect() const
	{
		return aspect;
	}

	float GetFovY() const
	{
		return fovY;
	}

	float GetFovX() const
	{
		float halfWidth = 0.5f*GetNearWindowHeight();
		return 2.0f*atan(halfWidth / nearZ);
	}

	float GetNearWindowWidth() const
	{
		return aspect * nearWindowHeight;
	}

	float GetNearWindowHeight() const
	{
		return nearWindowHeight;
	}

	float GetFarWindowWidth() const
	{
		return aspect * farWindowHeight;
	}

	float GetFarWindowHeight() const
	{
		return farWindowHeight;
	}

	void SetLens(float _fovY, float _aspect, float _zn, float _zf)
	{
		fovY = _fovY;
		aspect = _aspect;
		nearZ = _zn;
		farZ = _zf;

		nearWindowHeight = 2.0f * nearZ * tanf(0.5f*fovY);
		farWindowHeight = 2.0f * farZ * tanf(0.5f*fovY);

		proj = glm::perspective(fovY, aspect, nearZ, farZ);

	}

	void LookAt(glm::vec3 _pos, glm::vec3 _target, glm::vec3 _worldUp)
	{
		auto mat = glm::lookAt(_pos, _target, up);

		position = _pos;
		look = glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
		up = glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
		right = glm::vec3(mat[0][0], mat[1][0], mat[2][0]);


		viewDirty = true;
	}

	glm::mat4 GetView() const
	{
		return view;
	}

	glm::mat4 GetProj() const
	{
		return proj;
	}

	//Movement

	bool isMoveable() const
	{
		return moveable;
	}

	void EnableMove()
	{
		moveable = true;
	}

	void DisableMove()
	{
		moveable = false;
	}

	void Strafe(float d)
	{
		if (moveable)
		{
			position += d*right;
			viewDirty = true;
		}
	}

	void Walk(float d)
	{
		if (moveable)
		{
			position += d*look;
			viewDirty = true;
		}
	}

	void Pitch(float angle)
	{
		if (moveable)
		{
			pitch = angle;
			viewDirty = true;
		}
	}

	void Yaw(float angle)
	{
		if (moveable)
		{
			yaw = angle;
			viewDirty = true;
		}
	}

	void UpdateView()
	{
		if (viewDirty)
		{

			glm::quat key_quat = glm::quat(glm::vec3(pitch, yaw, roll));
			//reset values
			pitch = yaw = roll = 0.0f;

			//order matters,update camera_quat
			camera_quat = key_quat * camera_quat;
			camera_quat = glm::normalize(camera_quat);
			glm::mat4 rotate = glm::mat4_cast(camera_quat);

			auto translate = glm::translate(glm::mat4(), -position);
			view = rotate * translate;

			look = glm::vec3(view[0][2], view[1][2], view[2][2]);
			up = glm::vec3(view[0][1], view[1][1], view[2][1]);
			right = glm::vec3(view[0][0], view[1][0], view[2][0]);


			viewDirty = false;
		}
	}


private:

	glm::vec3	position{ 0.0f,0.0f,0.0f };
	glm::vec3	right{ 1.0f,0.0f,0.0f };
	glm::vec3	up{ 0.0f,1.0f,0.0f };
	glm::vec3	look{ 0.0f,0.0f,1.0f };

	float		nearZ{ 0.0f };
	float		farZ{ 0.0f };
	float		aspect{ 0.0f };
	float		fovY{ 0.0f };
	float		nearWindowHeight{ 0.0f };
	float		farWindowHeight{ 0.0f };

	glm::quat	camera_quat = glm::quat(1.0f,0.0f,0.0f,0.0f);
	float		pitch{ 0.0f }; 
	float		yaw{ 0.0f };
	float		roll{ 0.0f };

	bool moveable{ true };
	bool viewDirty{ true };

	glm::mat4 view = glm::mat4();
	glm::mat4 proj = glm::mat4();
};

