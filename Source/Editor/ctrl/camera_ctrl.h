#pragma once
#include "Urho3D/Scene/Node.h"
#include "Urho3D/Math/Vector3.h"


namespace urho3d
{
	namespace editor
	{
		class CameraCtrl
		{
		public:
			CameraCtrl(Urho3D::Node* cam);
			~CameraCtrl();
			void onPointerDown(int x, int y);
			void onPointerHover(int x, int y);
			void onPointerMove(int x, int y);
			void onPointerUp(int x, int y);
			void onWheel(float delta);
			void rotate(int x,int y);
			void move(int x,int y);
			void update();
			void pause(bool isp);
			void resetCamera();
			void gameView();
			void setParam(float fov, Urho3D::Vector3 angle, float dis);
			void setGameParam(float fov, Urho3D::Vector3 angle, float dis);
			void setCameraPos(Urho3D::Vector3& pos);
			Urho3D::Vector3 getLookAtPos();
		protected:
			int lastMouseX;
			int lastMouseY;
			bool isPause;
			Urho3D::Node* _camera;
		private:
			int begin_x;
			int begin_y;
			float curent_x_angle;
			float curent_y_angle;
			float begin_x_angle;
			float begin_y_angle;
			float game_x_angle = 33.1667f;
			float game_y_angle = -45.0f;
			float game_fov = 42.0f;
			const float edittor_dis = 8.8f;
			float game_dis = 170.0f;
			Urho3D::Vector3 look_pos;
		};
	}
}