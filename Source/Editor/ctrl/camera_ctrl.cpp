#include <QGuiApplication>
#include "camera_ctrl.h"
#include "Urho3D/Math/Plane.h"
#include "global_event.h"

using namespace Urho3D;

namespace urho3d
{
	namespace editor
	{
		CameraCtrl::CameraCtrl(Node* cam)
		{
			_camera = cam;
			lastMouseX = -1;
			lastMouseY = -1;
			isPause = false;
			curent_x_angle = _camera->GetRotation().EulerAngles().x_;
			curent_y_angle = _camera->GetRotation().EulerAngles().y_;
			look_pos = Vector3(0.0f, 5.0f, 0.0f);
			resetCamera();
		}

		CameraCtrl::~CameraCtrl()
		{
		}

		void CameraCtrl::onWheel(float delta)
		{
			float wheel_scale = 0.01;
			Vector3 oldPos = _camera->GetPosition();
			Vector3 newPos = oldPos + _camera->GetDirection()* delta * wheel_scale;
			_camera->SetPosition(newPos);
		}

		void CameraCtrl::onPointerDown(int x, int y)
		{
			if (isPause)
				return;
			lastMouseX = -1;
			lastMouseY = -1;
			begin_x = x;
			begin_y = y;
			begin_x_angle = curent_x_angle;
			begin_y_angle = curent_y_angle;

		}
		void CameraCtrl::onPointerHover(int x, int y)
		{
			
		}

		void CameraCtrl::rotate(int x, int y)
		{
			global_event::get_instance()->emit_event(eGlobalEventType::SetCursor,new event_data("SizeAllCursor"));
			//this->setCursor(Qt::CursorShape::SizeAllCursor);
			//EventMgr::GetInstance()->emitEvent(eGlobalEventType::SetCursor, new EventData(std::string("SizeAllCursor")));
			//calc look at pos
			/*Vector3 camera_pos = camera->GetPosition();
			Vector3 forward = camera->GetDirection();
			auto dist =  -camera_pos.y_ / forward.y_;
			Vector3 lookat_pos = camera_pos + forward *dist;
			curent_x_angle = begin_x_angle - (y - begin_y) * 0.05;
			curent_y_angle = begin_y_angle + (x - begin_x) * 0.1;
			Quaternion rot;
			rot.FromEulerAngles(curent_x_angle, curent_y_angle,0);
			camera->SetRotation(rot);
			Vector3 new_forward = camera->GetDirection();
			auto new_camera_pos = lookat_pos - new_forward * dist;
			setCameraPos(new_camera_pos);*/
			Vector3 lookPos = getLookAtPos();
			float detaX = x - lastMouseX;
			float detaY = y - lastMouseY;
			float speed = 0.1;
			Vector3 axisY(0, detaX, 0);
			Vector3 axisX = _camera->GetRight().Normalized() * detaY;
			Vector3 axis = axisX + axisY;
			float angle = axis.Length() * speed;
			Quaternion rot;
			rot.FromAngleAxis(angle, axis);//绕着某条轴旋转一定角度
			Vector3 newPosLocal = rot * (_camera->GetWorldPosition() - lookPos);
			Vector3 newPos = newPosLocal + lookPos;
			_camera->SetWorldPosition(newPos);
			_camera->SetWorldRotation((rot)*_camera->GetRotation());
		}

		void CameraCtrl::move(int x, int y)
		{
			Vector3 look_pos = getLookAtPos();
			int detaX = x - lastMouseX;
			int detaY = y - lastMouseY;
			//this->setCursor(Qt::CursorShape::ClosedHandCursor);
			//EventMgr::GetInstance()->emitEvent(eGlobalEventType::SetCursor, new EventData(std::string("ClosedHandCursor")));
			global_event::get_instance()->emit_event(eGlobalEventType::SetCursor, new event_data("ClosedHandCursor"));
			Vector3 oldPos = _camera->GetPosition();
			Quaternion  oldrot = _camera->GetRotation();
			Vector3 xDir = oldrot * Vector3(1.0, 0, 0);
			Vector3 yDir = oldrot * Vector3(0, -1, 0);
			Vector3 newPos(oldPos.x_, oldPos.y_, oldPos.z_);
			newPos = newPos - xDir * detaX * (oldPos - look_pos).Length() * 0.0008;
			newPos = newPos - yDir * detaY * (oldPos - look_pos).Length() * 0.0008;
			_camera->SetPosition(newPos);
		}

		void CameraCtrl::onPointerMove(int x, int y)
		{
			if (isPause)
				return;
			if (lastMouseY >= 0)
			{
				//if (SceneCtrl::GetInstance()->keyState->Alt)
				if(QGuiApplication::keyboardModifiers() == Qt::AltModifier)
				{					
					rotate(x, y);
				}
				else
				{
					move(x, y);
				}
			}
			lastMouseX = x;
			lastMouseY = y;
		}
		void CameraCtrl::onPointerUp(int x, int y)
		{
			global_event::get_instance()->emit_event(eGlobalEventType::SetCursor, new event_data("Normal"));
		}

		void CameraCtrl::update()
		{

		}
		void CameraCtrl::pause(bool isp)
		{
			isPause = isp;
		}
		void CameraCtrl::resetCamera()
		{
			/*curent_x_angle = game_x_angle;
			curent_y_angle = game_y_angle;
			camera->set_fov_angle(game_fov);
			camera->set_pos(vec3d(0,0,0));
			camera->set_angles(vec3d(game_x_angle, game_y_angle,0));
			auto forward = camera->get_forward_dir();
			auto camera_pos = vec3d(0, 0, 0) - forward * edittor_dis;*/
			//setCameraPos(camera_pos);
		}
		void CameraCtrl::gameView()
		{
			/*curent_x_angle = game_x_angle;
			curent_y_angle = game_y_angle;
			camera->set_fov_angle(game_fov);
			camera->set_pos(vec3d(0, 0, 0));
			camera->set_angles(vec3d(game_x_angle, game_y_angle, 0));
			auto forward = camera->get_forward_dir();
			auto camera_pos = vec3d(0, 0, 0) - forward * game_dis;*/
			//setCameraPos(camera_pos);

		}

		void CameraCtrl::setGameParam(float fov, Vector3 angle, float dis)
		{
			game_x_angle = angle.x_;
			game_y_angle = angle.y_;
			game_fov = fov;
			game_dis = dis;
		}

		void CameraCtrl::setParam(float fov, Vector3 angle, float dis)
		{
			/*camera->set_fov_angle(fov);
			camera->set_angles(angle);
			auto forward = camera->get_forward_dir();
			auto camera_pos = vec3d(0, 0, 0) - forward * dis;
			setCameraPos(camera_pos);*/
		}

		void CameraCtrl::setCameraPos(Vector3& pos)
		{
			/*camera->set_pos(pos);
			EventMgr::GetInstance()->emitEvent(eGlobalEventType::CameraPosChanged);*/
		}

		Vector3 CameraCtrl::getLookAtPos()
		{
			//相机方向和地面的交点
			Vector3 planeNormal(0, 1, 0);
			Vector3 planePoint(0, 0, 0);
			Vector3 point = _camera->GetWorldPosition();
			Vector3 direct = _camera->GetWorldDirection();
			//float d = Vector3.Dot(planePoint - point, planeNormal) / Vector3.Dot(direct.normalized, planeNormal);
			float d = planeNormal.DotProduct(planePoint - point) / planeNormal.DotProduct(direct.Normalized());
			Vector3 result = d * direct.Normalized() + point;
			return result;
		}
	}
}