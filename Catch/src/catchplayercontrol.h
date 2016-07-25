#ifndef _catchplayercontrol_h
#define _catchplayercontrol_h

#include <mezzanine.h>

using namespace Mezzanine;

class CatchPlayerControl
{
protected:
#ifdef MEZZDEBUG
    /// @brief The freeform controller for the camera to debug scenes.
    CameraController CamControl;
#endif
    /// @brief A pointer to the camera of the player.
    Graphics::CameraProxy* CatchPlayerView;
public:
    /// @brief Class constructor.
    CatchPlayerControl();
    /// @brief Class destructor.
    ~CatchPlayerControl();

    /// @brief Sets the camera to update.
    /// @param View A pointer to the camera of the players view.
    void SetCamera(Graphics::CameraProxy* View);
    /// @brief Gets the camera to update.
    /// @return Returns a pointer to the players camera.
    Graphics::CameraProxy* GetCamera() const;

    /// @brief Moves the camera along the track to the next node.
    /// @param Amount The amount of track advancement to apply.
    void MoveToNextTrackNode(Real Amount);
    /// @brief Moves the camera along the track to the previous node.
    /// @param Amount The amount of track advancement to apply.
    void MoveToPrevTrackNode(Real Amount);

#ifdef MEZZDEBUG
    /// @brief Moves the camera forward.
    /// @param Units The distance to be moved in world units.
    void MoveForward(Real Units);
    /// @brief Moves the camera backward.
    /// @param Units The distance to be moved in world units.
    void MoveBackward(Real Units);
    /// @brief Moves the camera to the left.
    /// @param Units The distance to be moved in world units.
    void StrafeLeft(Real Units);
    /// @brief Moves the camera to the right.
    /// @param Units The distance to be moved in world units.
    void StrafeRight(Real Units);
    /// @brief Rotates the camera.
    /// @details This is a safer rotation method that applies all the checks and can lock behaviors
    /// such as roll if configured to do so.
    /// @param Yaw The amount to rotate the camera on it's local Y axis in Radians.
    /// @param Pitch The amount to rotate the camera on it's local X axis in Radians.
    /// @param Roll The amount to rotate the camera on it's local Z axis in Radians.
    void Rotate(Real Yaw, Real Pitch, Real Roll);
#endif
};//CatchPlayer

#endif
