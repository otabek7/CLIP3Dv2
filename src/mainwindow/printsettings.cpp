#include "printsettings.h"
#include "qlocale.h"
#include "ui_printsettings.h"
#include <QFileDialog>
#include <QTextStream>

QString ImageFileDirectory2;
QString PrintScriptDirectory;

printsettings::printsettings(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::printsettings)
{
  ui->setupUi(this);
}

printsettings::~printsettings()
{
  delete ui;
}

void printsettings::initSettingsPointers(PrintSettings *pPrintSettings, PrintControls *pPrintControls, PrintScripts *pPrintScript, InjectionSettings *pInjectionSettings)
{
  psPrintSettings = pPrintSettings;
  psPrintControls = pPrintControls;
  psPrintScript = pPrintScript;
  psInjectionSettings = pInjectionSettings;
}

/*******************************General Print Settings*********************************************/
void printsettings::on_resinSelect_editTextChanged(const QString &arg1)
{
  psPrintSettings->Resin = arg1;
}

void printsettings::on_BitDepthParam_valueChanged(int arg1)
{
  psPrintSettings->BitMode = arg1;
}

void printsettings::on_MaxImageUpload_valueChanged(int arg1)
{
  int MaxImageVal = arg1;
  // Make sure that max image upload is high enough for initial exposure
  if (MaxImageVal < (psPrintSettings->InitialExposure / 5) + 1)
  {
    MaxImageVal = (psPrintSettings->InitialExposure / 5) + 1;
  }
  else if (MaxImageVal > 395)
  {
    MaxImageVal = 395;
  }
  psPrintSettings->MaxImageUpload = MaxImageVal;
}

void printsettings::on_StartingPositionParam_valueChanged(double arg1)
{
  psPrintSettings->StartingPosition = arg1;
}

void printsettings::on_LayerThicknessParam_valueChanged(double arg1)
{
  psPrintSettings->LayerThickness = arg1 / 1000;
}

void printsettings::on_ResyncRateList_currentIndexChanged(const QString &arg1)
{
  psPrintSettings->ResyncVP = arg1.toInt();
}

void printsettings::on_PostExposureDelayParam_valueChanged(double arg1)
{
  if (arg1 <= psPrintSettings->DarkTime)
  {
    psPrintSettings->PostExposureDelay = arg1;
  }
  else
  {
    ui->PostExposureDelayParam->setValue(0);
  }
}

void printsettings::on_JerkTimeParam_valueChanged(double arg1)
{
  psPrintSettings->JerkTime = arg1 / 1000; // convert from ms to s
}
/*********************************Light Engine Control*********************************************/
void printsettings::on_InitialExposureParameter_valueChanged(double arg1)
{
  psPrintSettings->InitialExposure = arg1;
}
void printsettings::on_InitialDelayParam_valueChanged(int arg1)
{
  psPrintSettings->InitialDelay = arg1;
}

void printsettings::on_UVIntensityParam_valueChanged(int arg1)
{
  psPrintSettings->UVIntensity = arg1;
}

void printsettings::on_InitialExposureIntensityParam_valueChanged(int arg1)
{
  psPrintSettings->InitialIntensity = arg1;
}

void printsettings::on_ExposureTimeParam_valueChanged(double arg1)
{
  // Multiply by 1000 to scale from ms to us
  psPrintSettings->ExposureTime = arg1 * 1000;
}

void printsettings::on_DarkTimeParam_valueChanged(double arg1)
{
  // Multiply by 1000 to scale from ms to us
  psPrintSettings->DarkTime = arg1 * 1000;
}

/***********************************Stage Control**************************************************/
void printsettings::on_ContinuousMotionButton_clicked()
{
  if (ui->ContinuousMotionButton->isChecked() == true)
  {
    psPrintSettings->MotionMode = CONTINUOUS;
    ui->SteppedMotionButton->setChecked(false);
    emit SettingsPrint("Continuous Motion Selected");
    // Dark time does not exist in continuous mode so disable it and set to 0
    psPrintSettings->DarkTime = 0;
    EnableParameter(DARK_TIME, OFF);
    ui->DarkTimeParam->setValue(0);
  }
}

void printsettings::on_SteppedMotionButton_clicked()
{
  if (ui->SteppedMotionButton->isChecked() == true)
  {
    psPrintSettings->MotionMode = STEPPED;
    emit SettingsPrint("Stepped Motion Selected");
    EnableParameter(DARK_TIME, ON);
  }
}

void printsettings::on_pumpingCheckBox_clicked()
{
  bool isChecked = ui->pumpingCheckBox->isChecked();
  psPrintSettings->PumpingMode = isChecked;
  EnableParameter(PUMP_HEIGHT, isChecked);
}

void printsettings::on_pumpingParameter_valueChanged(double arg1)
{
  if (psPrintSettings->PumpingMode == ON)
  {
    // divide by 1000 to scale from um to mm for stage input
    psPrintSettings->PumpingParameter = arg1 / 1000;
  }
  else
  {
    emit SettingsPrint("Please enable pumping before setting pumping parameter");
  }
}

void printsettings::on_StageVelocityParam_valueChanged(double arg1)
{
  psPrintSettings->StageVelocity = arg1;
}

void printsettings::on_StageAccelParam_valueChanged(double arg1)
{
  psPrintSettings->StageAcceleration = arg1;
}

void printsettings::on_MaxEndOfRunParam_valueChanged(double arg1)
{
  psPrintSettings->MaxEndOfRun = arg1;
}

void printsettings::on_MinEndOfRunParam_valueChanged(double arg1)
{
  psPrintSettings->MinEndOfRun = arg1;
}

/***********************************Injection Control**********************************************/
void printsettings::on_ContinuousInjection_clicked()
{
  if (ui->ContinuousInjection->isChecked() == true)
  {
    ui->SteppedContInjection->setChecked(false);
    psInjectionSettings->ContinuousInjection = ON;
    EnableParameter(BASE_INJECTION, ON);
    emit SettingsPrint("Continuouus injection selected");
  }
  else
  {
    psInjectionSettings->ContinuousInjection = false;
    emit SettingsPrint("Continuous injection disabled");
  }
}

void printsettings::on_SteppedContInjection_clicked()
{
  if (ui->SteppedContInjection->isChecked() == true)
  {
    ui->ContinuousInjection->setChecked(false);
    psInjectionSettings->SteppedContinuousInjection = ON;
    EnableParameter(BASE_INJECTION, ON);
  }
  else
  {
    psInjectionSettings->SteppedContinuousInjection = false;
    emit SettingsPrint("Stepped continuous injection disabled");
  }
}

void printsettings::on_BaseInfusionParam_valueChanged(double arg1)
{
  psInjectionSettings->BaseInjectionRate = arg1;
}

void printsettings::on_PreMovementCheckbox_clicked()
{
  if (ui->PreMovementCheckbox->isChecked() == true)
  {
    ui->PostMovementCheckbox->setChecked(false);
    psInjectionSettings->InjectionDelayFlag = PRE;
    emit SettingsPrint("Pre-Injection delay enabled");
  }
  else
  {
    ui->PreMovementCheckbox->setChecked(false);
    psInjectionSettings->InjectionDelayFlag = OFF;
    emit SettingsPrint("Injection delay disabled");
  }
}

void printsettings::on_PostMovementCheckbox_clicked()
{
  if (ui->PostMovementCheckbox->isChecked() == true)
  {
    ui->PreMovementCheckbox->setChecked(false);
    psInjectionSettings->InjectionDelayFlag = POST;
    emit SettingsPrint("Post-Injection delay enabled");
  }
  else
  {
    ui->PostMovementCheckbox->setChecked(false);
    psInjectionSettings->InjectionDelayFlag = OFF;
    emit SettingsPrint("Injection delay disabled");
  }
}

void printsettings::on_InjectionDelayParam_valueChanged(double arg1)
{
  psInjectionSettings->InjectionDelayParam = arg1;
}

void printsettings::on_InjectionRateParam_valueChanged(double arg1)
{
  psInjectionSettings->InfusionRate = arg1;
}

void printsettings::on_VolPerLayerParam_valueChanged(double arg1)
{
  psInjectionSettings->InfusionVolume = arg1;
}

void printsettings::on_InitialVolumeParam_valueChanged(double arg1)
{
  psInjectionSettings->InitialVolume = arg1;
}

/***********************************Dynamic Print Mode*********************************************/
void printsettings::on_UsePrintScript_clicked()
{
  bool toggle;
  bool script;
  if (ui->UsePrintScript->isChecked())
  {
    psPrintScript->PrintScript = ON;
    script = ON;
    toggle = OFF;
  }
  else
  {
    psPrintScript->PrintScript = OFF;
    script = OFF;
    toggle = ON;
  }
  ui->SelectPrintScript->setEnabled(script);
  ui->ClearPrintScript->setEnabled(script);
  ui->PrintScriptFile->setEnabled(script);

  EnableParameter(EXPOSURE_TIME, toggle); // Enable all user inputs for parameters that
  EnableParameter(LED_INTENSITY, toggle); // are not controlled by the print script
  EnableParameter(DARK_TIME, toggle);
  EnableParameter(LAYER_THICKNESS, toggle);
  EnableParameter(STAGE_VELOCITY, toggle);
  EnableParameter(STAGE_ACCELERATION, toggle);
  EnableParameter(PUMP_HEIGHT, toggle);
  EnableParameter(INJECTION_VOLUME, toggle);
  EnableParameter(INJECTION_RATE, toggle);
}

void printsettings::on_SelectPrintScript_clicked()
{
  QString file_name = QFileDialog::getOpenFileName(this, "Open Print Script", PrintScriptDirectory, "*.txt *.csv");
  if (file_name != "")
  {
    initPrintScript(file_name);
  }
}

void printsettings::initPrintScript(QString file_name)
{
  ui->PrintScriptFile->setText(file_name);
  QDir Directory = QFileInfo(file_name.at(0)).absoluteDir();  // Get directory selected
  PrintScriptDirectory = Directory.filePath(file_name.at(0)); // Save directory

  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly))
  {
    // qDebug() << file.errorString();
  }
  QStringList wordList;
  while (!file.atEnd())
  { // Runs until the end of the file
    QByteArray line = file.readLine();
    psPrintScript->ExposureScriptList.append(line.split(',').at(0));
    psPrintScript->LEDScriptList.append(line.split(',').at(1));
    psPrintScript->DarkTimeScriptList.append(line.split(',').at(2));
    psPrintScript->LayerThicknessScriptList.append(line.split(',').at(3));
    psPrintScript->StageVelocityScriptList.append(line.split(',').at(4));
    psPrintScript->StageAccelerationScriptList.append(line.split(',').at(5));
    psPrintScript->PumpHeightScriptList.append(line.split(',').at(6));
    if (psPrintSettings->PrinterType == ICLIP)
    {
      psPrintScript->InjectionVolumeScriptList.append(line.split(',').at(7));
      psPrintScript->InjectionRateScriptList.append(line.split(',').at(8));
    }
  }
  emit SettingsPrint("Print List has: " + QString::number(psPrintScript->ExposureScriptList.size()) + " exposure time entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScript->LEDScriptList.size()) + " LED intensity entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScript->DarkTimeScriptList.size()) + " dark time entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScript->LayerThicknessScriptList.size()) + " layer thickness entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScript->StageVelocityScriptList.size()) + " stage velocity entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScript->StageAccelerationScriptList.size()) + " stage acceleration entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScript->PumpHeightScriptList.size()) + +" pump height entries");

  emit updateScript();
}

void printsettings::on_ClearPrintScript_clicked()
{
  ui->PrintScriptFile->clear();
  psPrintScript->ExposureScriptList.clear();
  psPrintScript->LEDScriptList.clear();
  psPrintScript->DarkTimeScriptList.clear();
  psPrintScript->LayerThicknessScriptList.clear();
  psPrintScript->StageVelocityScriptList.clear();
  psPrintScript->StageAccelerationScriptList.clear();
  psPrintScript->PumpHeightScriptList.clear();
  if (psPrintSettings->PrinterType == ICLIP)
  {
    psPrintScript->InjectionVolumeScriptList.clear();
    psPrintScript->InjectionRateScriptList.clear();
  }
  emit updateScript();
}

/**********************************Image File Selection********************************************/
void printsettings::on_SelectFile_clicked()
{
  // Open files from last directory chosen, limited to bitmapped or tiff image file formats
  QStringList file_names = QFileDialog::getOpenFileNames(this, "Open Object Image Files", ImageFileDirectory2, "*.bmp *.png *.tiff *.tif");
  initImageFiles(file_names);
}

void printsettings::initImageFiles(QStringList file_names)
{
  if (file_names.count() > 0)
  {                                                                  // If images selected
    QDir ImageDirectory = QFileInfo(file_names.at(0)).absoluteDir(); // Get directory selected
    ImageFileDirectory2 = ImageDirectory.filePath(file_names.at(0)); // Save directory
    // Add each file name to FileList, will be displayed for user
    for (int i = 0; i < file_names.count(); i++)
    {
      ui->FileList->addItem(file_names.at(i));
    }
  }
  else
  { // No images were selected
    emit SettingsPrint("Please select more images");
  }
  int SliceCount = ui->FileList->count();
  emit SettingsPrint(QString::number(SliceCount) + " Images Currently Selected");
}

void printsettings::on_ClearImageFiles_clicked()
{
  ui->FileList->clear();
}

int printsettings::FileListCount()
{
  return ui->FileList->count();
}

QString printsettings::FileListItem(int itemNum)
{
  return ui->FileList->item(itemNum)->text();
}
/************************************Helper Functions**********************************************/
void printsettings::EnableParameter(Parameter_t Parameter, bool State)
{
  switch (Parameter)
  {
  case EXPOSURE_TIME:
    ui->ExposureTimeParam->setEnabled(State);
    ui->ExposureTimeBox->setEnabled(State);
    break;
  case LED_INTENSITY:
    ui->UVIntensityParam->setEnabled(State);
    ui->UVIntensityBox->setEnabled(State);
    break;
  case DARK_TIME:
    ui->DarkTimeParam->setEnabled(State);
    ui->DarkTimeBox->setEnabled(State);
    break;
  case LAYER_THICKNESS:
    ui->LayerThicknessParam->setEnabled(State);
    ui->LayerThicknessBox->setEnabled(State);
    break;
  case STAGE_VELOCITY:
    ui->StageVelocityParam->setEnabled(State);
    ui->StageVelocityBox->setEnabled(State);
    break;
  case STAGE_ACCELERATION:
    ui->StageAccelParam->setEnabled(State);
    ui->StageAccelerationBox->setEnabled(State);
    break;
  case PUMP_HEIGHT:
    ui->pumpingParameter->setEnabled(State);
    break;
  case INJECTION_VOLUME:
    ui->VolPerLayerParam->setEnabled(State);
    ui->VolPerLayerBox->setEnabled(State);
    break;
  case INJECTION_RATE:
    ui->InjectionRateParam->setEnabled(State);
    ui->InjectionRateBox->setEnabled(State);
    break;
  case BASE_INJECTION:
    ui->BaseInfusionParam->setEnabled(State);
    ui->BaseInfusionRateTitle->setEnabled(State);
    break;
  case MAX_IMAGE:
    ui->MaxImageUpload->setEnabled(State);
    ui->MaxImageUploadBox->setEnabled(State);
    break;
  case VP_RESYNC:
    ui->ResyncRateList->setEnabled(State);
    ui->ResyncRateBox->setEnabled(State);
    break;
  case INITIAL_VOLUME:
    ui->InitialVolumeParam->setEnabled(State);
    ui->InitialVolumeBox->setEnabled(State);
    break;
  case INITIAL_INTENSITY:
    ui->InitialExposureIntensityParam->setEnabled(State);
    ui->InitialIntensityBox->setEnabled(State);
    break;
  case INITIAL_DELAY:
    ui->InitialDelayParam->setEnabled(State);
    ui->InitialDelayBox->setEnabled(State);
    break;
  case CONTINUOUS_INJECTION:
    ui->ContinuousInjection->setEnabled(State);
    ui->SteppedContInjection->setEnabled(State);
    ui->ContInjectionBox->setEnabled(State);
    break;
  case STARTING_POSITION:
    ui->StartingPositionParam->setEnabled(State);
    ui->StartingPositionBox->setEnabled(State);
    break;
  case MAX_END:
    ui->MaxEndOfRunParam->setEnabled(State);
    ui->MaxEndOfRunBox->setEnabled(State);
    break;
  case MIN_END:
    ui->MinEndOfRunParam->setEnabled(State);
    ui->MinEndOfRunBox->setEnabled(State);
    break;
  case INJECTION_DELAY:
    ui->InjectionDelayParam->setEnabled(State);
    ui->InjectionDelayBox->setEnabled(State);
    break;
  default:
    break;
  }
}

/**********************************Settings Functions**********************************************/
void printsettings::savePrintSettings()
{
  QSettings settings;
  settings.setValue("Resin", psPrintSettings->Resin);
  settings.setValue("BitMode", psPrintSettings->BitMode);
  settings.setValue("MaxImageUpload", psPrintSettings->MaxImageUpload);
  settings.setValue("ResyncVP", psPrintSettings->ResyncVP);
  settings.setValue("LayerThickness", psPrintSettings->LayerThickness);
  settings.setValue("StartingPosition", psPrintSettings->StartingPosition);
  settings.setValue("PostExposureDelay", psPrintSettings->PostExposureDelay);
  settings.setValue("JerkTime", psPrintSettings->JerkTime);

  settings.setValue("ProjectionMode", psPrintSettings->ProjectionMode);
  settings.setValue("InitialExposure", psPrintSettings->InitialExposure);
  settings.setValue("InitialDelay", psPrintSettings->InitialDelay);
  settings.setValue("InitialIntensity", psPrintSettings->InitialIntensity);
  settings.setValue("UVIntensity", psPrintSettings->UVIntensity);
  settings.setValue("ExposureTime", psPrintSettings->ExposureTime);
  settings.setValue("DarkTime", psPrintSettings->DarkTime);

  settings.setValue("MotionMode", psPrintSettings->MotionMode);
  settings.setValue("PumpingMode", psPrintSettings->PumpingMode);
  settings.setValue("PumpingParameter", psPrintSettings->PumpingParameter);
  settings.setValue("StageVelocity", psPrintSettings->StageVelocity);
  settings.setValue("StageAcceleration", psPrintSettings->StageAcceleration);
  settings.setValue("MaxEndOfRun", psPrintSettings->MaxEndOfRun);
  settings.setValue("MinEndOfRun", psPrintSettings->MinEndOfRun);

  settings.setValue("InfusionRate", psInjectionSettings->InfusionRate);
  settings.setValue("InfusionVolume", psInjectionSettings->InfusionVolume);
  settings.setValue("InitialVolume", psInjectionSettings->InfusionVolume);
  settings.setValue("BaseInjectionRate", psInjectionSettings->BaseInjectionRate);
  settings.setValue("InjectionDelay", psInjectionSettings->InjectionDelayParam);

  settings.setValue("PrintScriptActive", psPrintScript->PrintScript);
  settings.setValue("PrintScript", ui->PrintScriptFile->text());
  settings.setValue("PrintScriptDirectory", PrintScriptDirectory);
  settings.setValue("ImageFileDirectory", ImageFileDirectory2);
  QStringList ImageList;
  QListWidgetItem *item;
  for (int i = 0; i < ui->FileList->count(); i++)
  {
    item = ui->FileList->item(i);
    ImageList << item->text();
  }
  settings.setValue("ImageFiles", ImageList);
}

void printsettings::loadPrintSettings()
{
  QSettings settings;
  psPrintSettings->Resin = settings.value("Resin", "No resin selected").toString();
  psPrintSettings->BitMode = settings.value("BitMode", 1).toDouble();
  psPrintSettings->MaxImageUpload = settings.value("MaxImageUpload", 50).toDouble();
  psPrintSettings->ResyncVP = settings.value("ResyncVP", 24).toDouble();
  psPrintSettings->LayerThickness = settings.value("LayerThickness", 1).toDouble();
  psPrintSettings->StartingPosition = settings.value("StartingPosition", 5).toDouble();
  psPrintSettings->PostExposureDelay = settings.value("PostExposureDelay", 0).toDouble();
  psPrintSettings->JerkTime = settings.value("JerkTime", 0.04).toDouble();

  psPrintSettings->ProjectionMode = settings.value("ProjectionMode", POTF).toInt();
  psPrintSettings->InitialExposure = settings.value("InitialExposure", 10).toDouble();
  psPrintSettings->InitialDelay = settings.value("InitialDelay", 0).toInt();
  psPrintSettings->InitialIntensity = settings.value("InitialIntensity", 10).toInt();
  psPrintSettings->UVIntensity = settings.value("UVIntensity", 12).toDouble();
  psPrintSettings->ExposureTime = settings.value("ExposureTime", 1000).toDouble();
  psPrintSettings->DarkTime = settings.value("DarkTime", 1000).toDouble();

  psPrintSettings->MotionMode = settings.value("MotionMode", STEPPED).toDouble();
  psPrintSettings->PumpingMode = settings.value("PumpingMode", 0).toDouble();
  psPrintSettings->PumpingParameter = settings.value("PumpingParameter", 0).toDouble();
  psPrintSettings->StageVelocity = settings.value("StageVelocity", 10).toDouble();
  psPrintSettings->StageAcceleration = settings.value("StageAcceleration", 5).toDouble();
  psPrintSettings->MaxEndOfRun = settings.value("MaxEndOfRun", 60).toDouble();
  psPrintSettings->MinEndOfRun = settings.value("MinEndOfRun", 0).toDouble();

  psInjectionSettings->InfusionRate = settings.value("InfusionRate", 5).toDouble();
  psInjectionSettings->InfusionVolume = settings.value("InfusionVolume", 5).toDouble();
  psInjectionSettings->InitialVolume = settings.value("InitialVolume", 0).toDouble();
  psInjectionSettings->BaseInjectionRate = settings.value("BaseInjectionRate", 0).toDouble();
  psInjectionSettings->InjectionDelayParam = settings.value("InjectionDelay", 0).toDouble();

  ImageFileDirectory2 = settings.value("ImageFileDirectory", "C://").toString();
  PrintScriptDirectory = settings.value("PrintScriptDirectory", "C://").toString();
  psPrintScript->PrintScript = settings.value("PrintScriptActive", 0).toInt();

  // These are initialized here because they are not stored in variables only ui
  QString PrintScriptFile = settings.value("PrintScript", "").toString();
  if (PrintScriptFile != "" && psPrintScript->PrintScript == ON)
  {
    initPrintScript(PrintScriptFile);
  }
  QStringList ImageFiles = settings.value("ImageFiles", "").toStringList();
  if (ImageFiles.isEmpty() == false)
  { // Needs to be nested to avoid segmentation fault
    if (ImageFiles.at(0) != "")
    {
      initImageFiles(ImageFiles);
    }
  }
}

void printsettings::logCurrentSettingsToFile()
{
  QFile file("C:/Users/HOME-PC/Documents/CLIP3Dv2/settings_logDMD1.txt");
  if (file.open(QIODevice::WriteOnly | QIODevice::Append))
  {
    QTextStream out(&file);
    out << "DMD 1 Resin: " << psPrintSettings->Resin << "\n";
    out << "DMD 1 BitMode: " << psPrintSettings->BitMode << "\n";
    out << "DMD 1 MaxImageUpload: " << psPrintSettings->MaxImageUpload << "\n";
    out << "DMD 1 ResyncVP: " << psPrintSettings->ResyncVP << "\n";
    out << "DMD 1 LayerThickness: " << psPrintSettings->LayerThickness << "\n";
    out << "DMD 1 StartingPosition: " << psPrintSettings->StartingPosition << "\n";
    out << "DMD 1 PostExposureDelay: " << psPrintSettings->PostExposureDelay << "\n";
    out << "DMD 1 JerkTime: " << psPrintSettings->JerkTime << "\n";
    out << "DMD 1 InitialExposure: " << psPrintSettings->InitialExposure << "\n";
    out << "DMD 1 InitialDelay: " << psPrintSettings->InitialDelay << "\n";
    out << "DMD 1 InitialIntensity: " << psPrintSettings->InitialIntensity << "\n";
    out << "DMD 1 UVIntensity: " << psPrintSettings->UVIntensity << "\n";
    out << "DMD 1 ExposureTime: " << psPrintSettings->ExposureTime << "\n";
    out << "DMD 1 DarkTime: " << psPrintSettings->DarkTime << "\n";
    out << "DMD 1 MotionMode: " << psPrintSettings->MotionMode << "\n";
    out << "DMD 1 PumpingMode: " << psPrintSettings->PumpingMode << "\n";
    out << "DMD 1 StageVelocity: " << psPrintSettings->StageVelocity << "\n";
    out << "DMD 1 StageAcceleration: " << psPrintSettings->StageAcceleration << "\n";
    out << "DMD 1 MaxEndOfRun: " << psPrintSettings->MaxEndOfRun << "\n";
    out << "DMD 1 MinEndOfRun: " << psPrintSettings->MinEndOfRun << "\n";
    out << "DMD 1 InfusionRate: " << psInjectionSettings->InfusionRate << "\n";
    out << "DMD 1 InfusionVolume: " << psInjectionSettings->InfusionVolume << "\n";
    out << "DMD 1 InitialVolume: " << psInjectionSettings->InitialVolume << "\n";
    out << "DMD 1 BaseInjectionRate: " << psInjectionSettings->BaseInjectionRate << "\n";
    out << "DMD 1 InjectionDelayParam: " << psInjectionSettings->InjectionDelayParam << "\n";
    out << "DMD 1 PrintScript: " << psPrintScript->PrintScript << "\n";
    file.close();
  }
}

void printsettings::initPrintSettings()
{
  // init resin
  bool FoundMatch = false;
  QStringList itemsInComboBox;
  for (int index = 0; index < ui->resinSelect->count(); index++)
  {
    if (psPrintSettings->Resin == ui->resinSelect->itemText(index))
    {
      ui->resinSelect->setCurrentIndex(index);
      FoundMatch = true;
      break;
    }
  }
  if (FoundMatch == false)
  {
    ui->resinSelect->addItem(psPrintSettings->Resin);
    ui->resinSelect->setCurrentIndex(ui->resinSelect->count() - 1);
  }
  ui->BitDepthParam->setValue(psPrintSettings->BitMode);
  ui->MaxImageUpload->setValue(psPrintSettings->MaxImageUpload);
  ui->ResyncRateList->setCurrentIndex((psPrintSettings->ResyncVP / 24) - 1);
  ui->LayerThicknessParam->setValue(psPrintSettings->LayerThickness * 1000);
  ui->StartingPositionParam->setValue(psPrintSettings->StartingPosition);
  ui->PostExposureDelayParam->setValue(psPrintSettings->PostExposureDelay);
  ui->JerkTimeParam->setValue(psPrintSettings->JerkTime * 1000); // convert from s to ms

  ui->InitialExposureParameter->setValue(psPrintSettings->InitialExposure);
  ui->InitialDelayParam->setValue(psPrintSettings->InitialDelay);
  ui->InitialExposureIntensityParam->setValue(psPrintSettings->InitialIntensity);
  ui->UVIntensityParam->setValue(psPrintSettings->UVIntensity);
  ui->ExposureTimeParam->setValue(psPrintSettings->ExposureTime / 1000);
  ui->DarkTimeParam->setValue(psPrintSettings->DarkTime / 1000);

  if (psPrintSettings->MotionMode == STEPPED)
  {
    ui->SteppedMotionButton->setChecked(true);
    ui->ContinuousMotionButton->setChecked(false);
    emit(on_SteppedMotionButton_clicked());
  }
  else if (psPrintSettings->MotionMode == CONTINUOUS)
  {
    ui->ContinuousMotionButton->setChecked(true);
    ui->SteppedMotionButton->setChecked(false);
    emit(on_ContinuousMotionButton_clicked());
  }

  if (psPrintSettings->PumpingMode == ON)
  {
    ui->pumpingCheckBox->setChecked(true);
    emit(on_pumpingCheckBox_clicked());
  }
  else if (psPrintSettings->PumpingMode == OFF)
  {
    ui->pumpingCheckBox->setChecked(false);
  }
  ui->StageVelocityParam->setValue(psPrintSettings->StageVelocity);
  ui->StageAccelParam->setValue(psPrintSettings->StageAcceleration);
  ui->MaxEndOfRunParam->setValue(psPrintSettings->MaxEndOfRun);
  ui->MinEndOfRunParam->setValue(psPrintSettings->MinEndOfRun);

  ui->InjectionRateParam->setValue(psInjectionSettings->InfusionRate);
  ui->VolPerLayerParam->setValue(psInjectionSettings->InfusionVolume);
  ui->InitialVolumeParam->setValue(psInjectionSettings->InfusionVolume);
  ui->BaseInfusionParam->setValue(psInjectionSettings->BaseInjectionRate);
  ui->InjectionDelayParam->setValue(psInjectionSettings->InjectionDelayParam);

  ui->UsePrintScript->setChecked(psPrintScript->PrintScript);
  if (psPrintScript->PrintScript == ON)
  {
    on_UsePrintScript_clicked(); // turn on print script
  }
  EnableParameter(VP_RESYNC, OFF);     // Always starts in POTF so ResyncVP is disabled from start
  EnableParameter(INITIAL_DELAY, OFF); // Always starts in POTF so Initial delay is disabled

  logCurrentSettingsToFile();
}

#if 0

#endif
