#pragma once

#include "stdafx.h"

//
int OnActionDown(TP_ACTION action);
int OnActionUp(TP_ACTION action);
int OnActionMove(TP_ACTION action);
int OnActionSingleTap(TP_ACTION action);
int OnActionSingleTapUp(TP_ACTION action);
int OnActionDoubleTapDown(TP_ACTION action);
int OnActionDoubleTapUp(TP_ACTION action);
int OnActionDoubleTapMove(TP_ACTION action);
int OnActionFling(TP_ACTION action);

//
int OnActionKeyboardDown(TP_ACTION action);
int OnActionKeyboardUp(TP_ACTION action);
int OnActionKeyboardPress(TP_ACTION action);
int OnActionKeyboardToggle(TP_ACTION action);

//
int Shutdown(int flag);