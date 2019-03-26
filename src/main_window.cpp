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
      m_durationLabel.ChangeLabelText("0 из " + ToString(duration) + " месяцев");
      return ControlFlow::Stop;
    },
    "9" /* default */
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
    },
    "200000" /* default */
  );

  for (auto const type : {Insurance::Type::Car, Insurance::Type::Health, Insurance::Type::Home})
  {
    ShowInsuranceInfo(type, true /* first */);
    CreateDemandLabel(type);
    m_demandLabels[type].ChangeLabelText(ToString(m_game.GetCompany().GetDemand(type)));
  }

  CreateStepButton();
}

void MainWindow::CreateDurationLabel()
{
  m_durationLabel.m_label = std::make_unique<QLabel>(this);
  m_durationLabel.m_label->setFrameStyle(QFrame::StyledPanel);
  m_durationLabel.m_title = "Прошло: ";

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

void MainWindow::ReadFromUser(std::string const & title, MainWindow::ReadFromUselCallback && callback,
                              std::string const & defaultMessage = "")
{
  for (;;)
  {
    auto dialog = std::make_unique<QInputDialog>();
    dialog->resize(400, 50);
    dialog->setTextValue(defaultMessage.c_str());
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
  m_demandLabels[type].m_label->setGeometry(QRect(w - 310, 100 + 40 * number, 300, 40));

  m_demandLabels[type].m_settingsButton = std::make_unique<QPushButton>("Посмотреть/Изменить", this);
  m_demandLabels[type].m_settingsButton->setGeometry(QRect(w - 530, 100 + 40 * number - 4, 200, 50));
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
    //if (!first)
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
    addField("Базовый спрос", ToString(m_game.GetCompany().GetDemand(type)));
  else
    addInfo("Текущий спрос: " + ToString(m_game.GetCompany().GetDemand(type)));

  auto & insurance = m_game.GetCompany().GetInsurance(type);
  addField("Размер страхового взноса за 1 месяц", insurance.GetMonthPaymentValue());
  addField("Длительность строховки в месяцах", insurance.GetContractTime());
  addField("Сумма макс. возмещения ущерба", insurance.GetMaxSatisfactionValue());
  addField("Минимальный ущерб с которого начинается возмещение страховки (число от 0 до 1)",
           insurance.GetMinDamage());

  auto * listWidget = new QListWidget(&dialog);
  auto * monthPayment = new QListWidgetItem(tr("Месячная"), listWidget);
  auto * quarterPayment = new QListWidgetItem(tr("Квартальная"), listWidget);
  auto * yearPayment = new QListWidgetItem(tr("Годовая"), listWidget);
  form.addRow("Выберите тип страховки", listWidget);

  switch (insurance.GetPaymentType())
  {
    case Insurance::PaymentType::Month:
      listWidget->setCurrentItem(monthPayment);
      break;
    case Insurance::PaymentType::Quarter:
      listWidget->setCurrentItem(quarterPayment);
      break;
    case Insurance::PaymentType::Year:
      listWidget->setCurrentItem(yearPayment);
      break;
    default:
      CHECK(false, ("Bad value:", static_cast<int>(insurance.GetPaymentType())));
  }

  // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
  auto ok = QDialogButtonBox::Ok;
  QDialogButtonBox buttonBox(first ? ok : ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
  form.addRow(&buttonBox);
  QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
  QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

  bool showAgain = false;
  bool saveFirst = first;

  Insurance::PaymentType paymentType;
  uint32_t demand;
  double monthPaymentValue;
  uint32_t contractTime;
  double maxSatisfactionValue;
  double minDamage;

  if (dialog.exec() == QDialog::Accepted)
  {

    auto selectedPaymentRange = listWidget->currentIndex();
    if (selectedPaymentRange.row() == -1)
    {
      std::cout << "Bad selectedPaymentRange = " << selectedPaymentRange.row() << std::endl;
      ShowMessage(ToString("Вы не выбрали тип страховки"));
      showAgain = true;
    }
    else
      paymentType = static_cast<Insurance::PaymentType>(selectedPaymentRange.row());

    // If the user didn't dismiss the dialog, do something with the fields
    uint32_t count = 0;
    for (auto * lineEdit : fields)
    {
      auto const & input = lineEdit->text().toStdString();
      if (first && count == 0)
      {
//        m_game.GetCompany().GetDemand(type) = FromString<uint32_t>(input);
        demand = FromString<uint32_t>(input);
        first = false;
        continue;
      }

      switch (count)
      {
      case 0:
      {
//        auto value = FromString<double>(input);
        monthPaymentValue = FromString<double>(input);
        if (monthPaymentValue <= 0)
        {
          std::cout << "Bad MonthPaymentValue data: " << monthPaymentValue << std::endl;
          ShowMessage(ToString("Месячная плата(", monthPaymentValue, ") меньше 0"));
          showAgain = true;
        }
        break;
      }
      case 1:
      {
        auto time = FromString<int32_t>(input);
        if (time > 0)
          contractTime = static_cast<uint32_t>(time);
        else
        {
          std::cout << "Bad ContractTime data: " << time << std::endl;
          ShowMessage(ToString("Срок действия страховки (", time, ") меньше 0"));
          showAgain = true;
        }
        break;
      }
      case 2:
      {
//        auto value = FromString<double>(input);
        maxSatisfactionValue = FromString<double>(input);
        if (maxSatisfactionValue <= 0)
        {
          std::cout << "Bad MaxSatisfactionValue data: " << maxSatisfactionValue << std::endl;
          ShowMessage(ToString("Сумма макс. возмещения (", maxSatisfactionValue, ") меньше 0"));
          showAgain = true;
        }
        break;
      }
      case 3:
      {
//        auto damage = FromString<double>(input);
        minDamage = FromString<double>(input);
        if (!(0 < minDamage && minDamage < 1))
        {
          std::cout << "Bad damage data: " << minDamage << std::endl;
          ShowMessage(ToString("Минимальный урон должен быть числом от 0 до 1, введено:", minDamage));
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
    return ShowInsuranceInfo(type, saveFirst);

  m_game.GetCompany().GetDemand(type) = demand;
  m_game.GetCompany().GetInsurance(type) = Insurance(paymentType,
                                                     monthPaymentValue,
                                                     contractTime,
                                                     maxSatisfactionValue,
                                                     minDamage);
}

void MainWindow::CreateStepButton()
{
  auto && [w, h] = m_windowSize;
  m_stepButton = std::make_unique<QPushButton>("Сделать шаг", this);
  m_stepButton->setGeometry(QRect(w - 250, 250, 200, 60));
  auto * ptr = m_stepButton.get();

  auto const handler = [this](QObject * sender, bool pressed) {
    std::cout << "clicked to step button" << std::endl;
    if (m_game.IsLose() || m_game.IsWin())
      return;

    m_game.DoStep();

    // Обновим текущий месяц
    m_durationLabel.ChangeLabelText(ToString(m_game.GetCurrentMonth()) +
                                    " из " +
                                    ToString(m_game.GetDuration()) + " месяцев");

    // Обновим капитал компании
    m_fundLabel.ChangeLabelText(ToString(m_game.GetCompany().GetCurrentFund()));

    // Обновим спрос на страховки и кол-во проданных
    for (auto && [type, demand] : m_game.GetCompany().GetDemands())
    {
      m_demandLabels[type].ChangeLabelText(ToString(demand) +
                                           "\nПродано: " +
                                           ToString(m_game.GetCompany().GetInsuranceSoldNumber(type)));
    }

    if (m_game.IsLose())
    {
      ShowMessage("Вы проиграли :(");
      return;
    }

    if (m_game.IsWin())
    {
      ShowMessage("Вы победили :)");
      return;
    }
  };

  QObject::connect(ptr, &QPushButton::clicked, std::bind(handler, ptr, std::placeholders::_1));
}

void MainWindow::ShowMessage(std::string const & message)
{
  QMessageBox msgBox;
  msgBox.setText(message.c_str());
  msgBox.exec();
}

void MainWindow::Label::ChangeLabelText(std::string const & toAppend)
{
  auto text = m_title + " " + toAppend;
  m_label->setText(text.c_str());
}
