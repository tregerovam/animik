/***************************************************************************
 *   Copyright (C) 2006 by Zi Ree   *
 *   Zi Ree @ SecondLife   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

/**
	@author Zi Ree <Zi Ree @ Second Life>
*/

#include <QString>



class Settings              //TODO: this doesn't have to be static, rather singleton
{
public:
  Settings();
  ~Settings();

  static int m_windowHeight;
  static int m_windowWidth;

  static QString m_lastPath;

  static bool m_fog;
  static int  m_floorTranslucency;

  static bool m_easeIn;       //TODO: animation
  static bool m_easeOut;      //      specific?

  static int windowHeight();
  static int windowWidth();

  static QString lastPath();

  static void readSettings();

//    static void setFog(bool on);
  static bool fog();

//    static void setFloorTranslucency(int value);
  static int  floorTranslucency();

//    static void setEaseIn(bool on);
  static bool easeIn();
//    static void setEaseOut(bool on);
  static bool easeOut();
};

#endif
