#pragma once
#include <iostream>
#include <vector>
#include <windows.h>

class Monitor {
  private:
    std::vector<MONITORINFO> infos;

  public:
    enum class Position {
      LeftTop,
      LeftCenter,
      LeftBottom,
      CenterTop,
      CenterCenter,
      CenterBottom,
      RightTop,
      RightCenter,
      RightBottom
    };

  private:
    static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData) {
      Monitor *pThis = reinterpret_cast<Monitor *>(pData);

      MONITORINFO info;
      info.cbSize = sizeof(info);
      GetMonitorInfo(hMon, &info);
      pThis->infos.push_back(info);

      return TRUE;
    }

  public:
    Monitor() {
      EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
      std::reverse(infos.begin(), infos.end());
    }

    int getCount() {
      return infos.size();
    }

    void getPoint(const int index, Position position, POINT &point) {
      switch (position) {
        case Position::LeftTop:
          point.x = infos[index].rcMonitor.left;
          point.y = infos[index].rcMonitor.top;
          break;
        case Position::LeftCenter:
          point.x = infos[index].rcMonitor.left;
          point.y = (infos[index].rcMonitor.top + infos[index].rcMonitor.bottom) / 2;
          break;
        case Position::LeftBottom:
          point.x = infos[index].rcMonitor.left;
          point.y = infos[index].rcMonitor.bottom;
          break;
        case Position::CenterTop:
          point.x = (infos[index].rcMonitor.left + infos[index].rcMonitor.right) / 2;
          point.y = infos[index].rcMonitor.top;
          break;
        case Position::CenterCenter:
          point.x = (infos[index].rcMonitor.left + infos[index].rcMonitor.right) / 2;
          point.y = (infos[index].rcMonitor.top + infos[index].rcMonitor.bottom) / 2;
          break;
        case Position::CenterBottom:
          point.x = (infos[index].rcMonitor.left + infos[index].rcMonitor.right) / 2;
          point.y = infos[index].rcMonitor.bottom;
          break;
        case Position::RightTop:
          point.x = infos[index].rcMonitor.right;
          point.y = infos[index].rcMonitor.top;
          break;
        case Position::RightCenter:
          point.x = infos[index].rcMonitor.right;
          point.y = (infos[index].rcMonitor.top + infos[index].rcMonitor.bottom) / 2;
          break;
        case Position::RightBottom:
          point.x = infos[index].rcMonitor.right;
          point.y = infos[index].rcMonitor.bottom;
          break;
      }
    }
};
