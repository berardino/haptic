/*
 * vtkFlRenderWindowInteractor - class to enable VTK to render to and interact
 * with a FLTK window.
 * 
 * Copyright (c) 2002 Charl P. Botha <cpbotha@ieee.org> http://cpbotha.net/
 * Based on original code and concept copyright (c) 2000,2001 David Pont
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 * 
 * See the .cxx for more notes.
 * 
 * $Id: vtkFlRenderWindowInteractor.h,v 1.9 2002/03/18 21:19:34 cpbotha Exp $
 */

#ifndef _vtkFlRenderWindowInteractor_h
#define _vtkFlRenderWindowInteractor_h

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.h>
#include <vtkRenderWindowInteractor.h>


class vtkFlRenderWindowInteractor : public Fl_Gl_Window, public vtkRenderWindowInteractor {
 protected:
   // Fl_Gl_Window overrides
   void flush(void);
   
   void resize( int x, int y, int w, int h );
   int  handle( int event );
   void draw_overlay(void);

 public:
	 virtual void OnMouseDrag();
	 virtual void MouseMove(int x,int y);
	 virtual void LeftButtonPress(int x,int y);
	 virtual void LeftButtonRelease(int x,int y);
	 virtual void RightButtonPress(int x,int y);
	 virtual void RightButtonRelease(int x,int y);
	 void draw( void );	
   // ctors
   vtkFlRenderWindowInteractor();
   vtkFlRenderWindowInteractor( int x, int y, int w, int h);
   // vtk ::New()
   static vtkFlRenderWindowInteractor * New();
   // dtor
   ~vtkFlRenderWindowInteractor( void );

   // vtkRenderWindowInteractor overrides
   void Initialize();
   void Enable();
   void Disable();
   void Start();
   void SetRenderWindow(vtkRenderWindow *aren);
   void UpdateSize(int x, int y);
   int CreateTimer(int timertype);
   int DestroyTimer();
   void OnTimer(void);
   void TerminateApp();
  };

#endif
