#include "main_window.hpp"

#include "base.hpp"

#include <functional>
#include <iostream>

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
  QDesktopWidget dw;
  auto x = static_cast<uint32_t>(dw.width() * 0.7);
  auto y = static_cast<uint32_t>(dw.height() * 0.7);

  m_windowSize = {x, y};
  setFixedSize(x,y);
  std::cout << "Window size, width: " << x << ", height: " << y << std::endl;

  ReadFromUser(
    "Введите продолжительность игры в месяцах (число от 6 до 24)",
    [&](std::string const & input) {
      auto duration = FromString<uint32_t>(input);
      if (!(6 <= duration && duration <= 24))
        return ControlFlow::Continue;

      m_game.SetGameDuration(duration);
      std::cout << "Game duration is set to: " << duration << std::endl;

      CreateDurationLabel();
      m_durationLabel.ChangeLabelText(ToString(duration) + " месяцев");
      return ControlFlow::Stop;
    }
  );

  ReadFromUser(
    "Введите стартовый капитан (целое число большее 0)",
    [&](std::string const & input) {
      auto fund = FromString<int32_t>(input);
      if (fund <= 0)
        return ControlFlow::Continue;

      m_game.SetStartFunc(fund);
      std::cout << "Game start fund is set to: " << fund << std::endl;

      CreateFundLabel();
      m_fundLabel.ChangeLabelText(ToString(fund));
      return ControlFlow::Stop;
    }
  );

  for (auto const type : {Insurance::Type::Car, Insurance::Type::Health, Insurance::Type::Home})
  {
    ShowInsuranceInfo(type, true /* first */);
    CreateDemandLabel(type);
    m_demandLabels[type].ChangeLabelText(ToString(m_game.GetCompany().GetDemand(type)));
  }
}

void MainWindow::CreateDurationLabel()
{
  m_durationLabel.m_label = std::make_unique<QLabel>(this);
  m_durationLabel.m_label->setFrameStyle(QFrame::StyledPanel);
  m_durationLabel.m_title = "Продолжительность игры:";

  auto && [w, _] = m_windowSize;
  m_durationLabel.m_label->setGeometry(QRect(w - 310, 10, 300, 30));
}

void MainWindow::CreateFundLabel()
{
  m_fundLabel.m_label = std::make_unique<QLabel>(this);
  m_fundLabel.m_label->setFrameStyle(QFrame::StyledPanel);
  m_fundLabel.m_title = "Капитал компании:";

  auto && [w, h] = m_windowSize;
  m_fundLabel.m_label->setGeometry(QRect(w - 310, 50, 300, 30));
}

void MainWindow::ReadFromUser(std::string const & title, MainWindow::ReadFromUselCallback && callback)
{
  for (;;)
  {
    auto dialog = std::make_unique<QInputDialog>();
    dialog->resize(400, 50);
    dialog->setLabelText(title.c_str());
    dialog->exec();

    std::string input = dialog->textValue().toUtf8().constData();
    if (callback(input) == ControlFlow::Stop)
      break;
  }
}

void MainWindow::CreateDemandLabel(Insurance::Type type)
{
  m_demandLabels[type].m_label = std::make_unique<QLabel>(this);
  m_demandLabels[type].m_label->setFrameStyle(QFrame::StyledPanel);

  m_demandLabels[type].m_title = "Спрос на " + ToString(type) + ": ";

  auto number = static_cast<uint32_t>(type);

  auto && [w, h] = m_windowSize;
  m_demandLabels[type].m_label->setGeometry(QRect(w - 310, 100 + 30 * number, 300, 30));

  m_demandLabels[type].m_settingsButton = std::make_unique<QPushButton>("Посмотреть/Изменить", this);
  m_demandLabels[type].m_settingsButton->setGeometry(QRect(w - 530, 100 + 30 * number, 200, 35));
  auto * ptr = m_demandLabels[type].m_settingsButton.get();

  auto const handler = [type, this](QObject * sender, bool pressed) {
    std::cout << "clicked to " << ToString(type) << std::endl;
    ShowInsuranceInfo(type, false /* first */);
  };

  QObject::connect(ptr, &QPushButton::clicked, std::bind(handler, ptr, std::placeholders::_1));
}

void MainWindow::ShowInsuranceInfo(Insurance::Type type, bool first = false)
{
  QDialog dialog(this);
  // Use a layout allowing to have a label next to each field
  QFormLayout form(&dialog);

  // Add some text above the fields
  form.addRow(new QLabel(std::string("Настройки страховки вида: " + ToString(type)).c_str()));

  // Add the lineEdits with their respective labels
  QList<QLineEdit *> fields;

  auto const addField = [&](std::string const & title, auto const & value)
  {
    auto * lineEdit = new QLineEdit(&dialog);
    QString label = QString(title.c_str());
    if (!first)
      lineEdit->setText(ToString(value).c_str());

    form.addRow(label, lineEdit);
    fields << lineEdit;
  };

  auto const addInfo = [&](std::string const & title)
  {
    auto * label = new QLabel(this);
    label->setAlignment(Qt::AlignmentFlag::AlignRight);
    label->setText(title.c_str());
    form.addRow(label);
  };

  if (first)
    addField("Базовый спрос", int());
  else
    addInfo("Текущий спрос: " + ToString(m_game.GetCompany().GetDemand(type)));

  auto & insurance = m_game.GetCompany().GetInsurance(type);
  addField("Размер страхового взноса за 1 месяц", insurance.GetMonthPaymentValue());
  addField("Длительность строховки в месяцах", insurance.GetContractTime());
  addField("Сумма макс. возмещения ущерба", insurance.GetMaxSatisfactionValue());
  addField("Минимальный ущерб с которого начинается возмещение страховки (число от 0 до 1)",
           insurance.GetMinDamage());

  // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
  auto ok = QDialogButtonBox::Ok;
  QDialogButtonBox buttonBox(first ? ok : ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
  form.addRow(&buttonBox);
  QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
  QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

  bool showAgain = false;
  bool saveFirst = first;
  // Show the dialog as modal
  if (dialog.exec() == QDialog::Accepted)
  {
    // If the user didn't dismiss the dialog, do something with the fields
    uint32_t count = 0;
    for (auto * lineEdit : fields)
    {
      auto const & input = lineEdit->text().toStdString();
      if (first && count == 0)
      {
        m_game.GetCompany().GetDemand(type) = FromString<uint32_t>(input);
        first = false;
        continue;
      }

      switch (count)
      {
      case 0:
      {
        auto value = FromString<double>(input);
        if (value > 0)
          insurance.GetMonthPaymentValue() = value;
        else
        {
          std::cout << "Bad MonthPaymentValue data: " << value << std::endl;
          showAgain = true;
        }
        break;
      }
      case 1:
      {
        auto time = FromString<int32_t>(input);
        if (time > 0)
          insurance.GetContractTime() = static_cast<uint32_t>(time);
        else
        {
          std::cout << "Bad ContractTime data: " << time << std::endl;
          showAgain = true;
        }
        break;
      }
      case 2:
      {
        auto value = FromString<double>(input);
        if (value > 0)
          insurance.GetMaxSatisfactionValue() = value;
        else
        {
          std::cout << "Bad MaxSatisfactionValue data: " << value << std::endl;
          showAgain = true;
        }
        break;
      }
      case 3:
      {
        auto damage = FromString<double>(input);
        if (0 < damage && damage < 1)
          insurance.GetMinDamage() = damage;
        else
        {
          std::cout << "Bad damage data: " << damage << std::endl;
          showAgain = true;
        }
        break;
      }
      default:
        CHECK(false, ("Error in dialog."));
      }

      ++count;
    }

    std::cout << "Insurance type[ " << ToString(type) << " ], data = {" << ToString(insurance) << "}" << std::endl;
  }
  else if (first)
  {
    showAgain = true;
  }

  if (showAgain)
    ShowInsuranceInfo(type, saveFirst);
}

void MainWindow::Label::ChangeLabelText(std::string const & toAppend)
{
  auto text = m_title + " " + toAppend;
  m_label->setText(text.c_str());
}
