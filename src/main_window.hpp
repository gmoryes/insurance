#pragma once

#include "game.hpp"

#include <map>
#include <memory>
#include <utility>

#include <QtWidgets>

class MainWindow : public QMainWindow
{
public:
  enum class ControlFlow
  {
    Continue,
    Stop
  };

  using ReadFromUselCallback = std::function<ControlFlow(std::string const &)>;

  explicit MainWindow(QWidget * parent = nullptr, Qt::WindowFlags flags = 0);

private:
  void ReadFromUser(std::string const & title, ReadFromUselCallback && callback, std::string const & defaultMessage);
  void CreateDurationLabel();
  void CreateFundLabel();
  void CreateDemandLabel(Insurance::Type type);
  void ShowInsuranceInfo(Insurance::Type type, bool first, bool forceUpdate);
  void CreateStepButton();
  void ShowMessage(std::string const & message);
  // W, H
  std::pair<uint32_t, uint32_t> m_windowSize;

  Game m_game;

  struct Label
	{
    void ChangeLabelText(std::string const & toAppend);
    std::unique_ptr<QLabel> m_label;
    std::unique_ptr<QPushButton> m_settingsButton;
    std::string m_title;
	};

  Label m_durationLabel;
  Label m_fundLabel;
  std::map<Insurance::Type, Label> m_demandLabels;
  std::unique_ptr<QPushButton> m_stepButton;
};
