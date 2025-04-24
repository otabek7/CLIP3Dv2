#include "printsettingsDMD2.h"
#include "ui_printsettingsDMD2.h"
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>

QString ImageFileDirectory3;
QString PrintScriptDirectory2;

printsettingsDMD2::printsettingsDMD2(QWidget *parent) : QWidget(parent),
                                                        ui(new Ui::printsettingsDMD2)
{
  ui->setupUi(this);
}

printsettingsDMD2::~printsettingsDMD2()
{
  delete ui;
}

void printsettingsDMD2::initSettingsPointers(PrintSettings *pPrintSettings, PrintControls *pPrintControls, PrintScripts *pPrintScript, InjectionSettings *pInjectionSettings)
{
  psPrintSettingsDMD2 = pPrintSettings;
  psPrintControlsDMD2 = pPrintControls;
  psPrintScriptDMD2 = pPrintScript;
  psInjectionSettingsDMD2 = pInjectionSettings;
}

/*******************************General Print Settings*********************************************/
void printsettingsDMD2::on_resinSelectDMD2_editTextChanged(const QString &arg1)
{
  psPrintSettingsDMD2->Resin = arg1;
}

void printsettingsDMD2::on_BitDepthParamDMD2_valueChanged(int arg1)
{
  psPrintSettingsDMD2->BitMode = arg1;
}

void printsettingsDMD2::on_MaxImageUploadDMD2_valueChanged(int arg1)
{
  int MaxImageVal = arg1;
  // Make sure that max image upload is high enough for initial exposure
  if (MaxImageVal < (psPrintSettingsDMD2->InitialExposure / 5) + 1)
  {
    MaxImageVal = (psPrintSettingsDMD2->InitialExposure / 5) + 1;
  }
  else if (MaxImageVal > 395)
  {
    MaxImageVal = 395;
  }
  psPrintSettingsDMD2->MaxImageUpload = MaxImageVal;
}

void printsettingsDMD2::on_StartingPositionParamDMD2_valueChanged(double arg1)
{
  psPrintSettingsDMD2->StartingPosition = arg1;
}

void printsettingsDMD2::on_LayerThicknessParamDMD2_valueChanged(double arg1)
{
  psPrintSettingsDMD2->LayerThickness = arg1 / 1000;
}

void printsettingsDMD2::on_ResyncRateListDMD2_currentIndexChanged(const QString &arg1)
{
  psPrintSettingsDMD2->ResyncVP = arg1.toInt();
}

void printsettingsDMD2::on_PostExposureDelayParamDMD2_valueChanged(double arg1)
{
  if (arg1 <= psPrintSettingsDMD2->DarkTime)
  {
    psPrintSettingsDMD2->PostExposureDelay = arg1;
  }
  else
  {
    ui->PostExposureDelayParamDMD2->setValue(0);
  }
}

void printsettingsDMD2::on_JerkTimeParamDMD2_valueChanged(double arg1)
{
  psPrintSettingsDMD2->JerkTime = arg1 / 1000; // convert from ms to s
}
/*********************************Light Engine Control*********************************************/
void printsettingsDMD2::on_InitialExposureParameterDMD2_valueChanged(double arg1)
{
  psPrintSettingsDMD2->InitialExposure = arg1;
}
void printsettingsDMD2::on_InitialDelayParamDMD2_valueChanged(int arg1)
{
  psPrintSettingsDMD2->InitialDelay = arg1;
}

void printsettingsDMD2::on_UVIntensityParamDMD2_valueChanged(int arg1)
{
  psPrintSettingsDMD2->UVIntensity = arg1;
}

void printsettingsDMD2::on_InitialExposureIntensityParamDMD2_valueChanged(int arg1)
{
  psPrintSettingsDMD2->InitialIntensity = arg1;
}

void printsettingsDMD2::on_ExposureTimeParamDMD2_valueChanged(double arg1)
{
  // Multiply by 1000 to scale from ms to us
  psPrintSettingsDMD2->ExposureTime = arg1 * 1000;
}

void printsettingsDMD2::on_DarkTimeParamDMD2_valueChanged(double arg1)
{
  // Multiply by 1000 to scale from ms to us
  psPrintSettingsDMD2->DarkTime = arg1 * 1000;
}

/***********************************Stage Control**************************************************/
void printsettingsDMD2::on_ContinuousMotionButtonDMD2_clicked()
{
  if (ui->ContinuousMotionButtonDMD2->isChecked() == true)
  {
    psPrintSettingsDMD2->MotionMode = CONTINUOUS;
    ui->SteppedMotionButtonDMD2->setChecked(false);
    emit SettingsPrint("Continuous Motion Selected");
    // Dark time does not exist in continuous mode so disable it and set to 0
    psPrintSettingsDMD2->DarkTime = 0;
    EnableParameter(DARK_TIME, OFF);
    ui->DarkTimeParamDMD2->setValue(0);
  }
}

void printsettingsDMD2::on_SteppedMotionButtonDMD2_clicked()
{
  if (ui->SteppedMotionButtonDMD2->isChecked() == true)
  {
    psPrintSettingsDMD2->MotionMode = STEPPED;
    emit SettingsPrint("Stepped Motion Selected");
    EnableParameter(DARK_TIME, ON);
  }
}

void printsettingsDMD2::on_pumpingCheckBoxDMD2_clicked()
{
  bool isChecked = ui->pumpingCheckBoxDMD2->isChecked();
  psPrintSettingsDMD2->PumpingMode = isChecked;
  EnableParameter(PUMP_HEIGHT, isChecked);
}

void printsettingsDMD2::on_pumpingParameterDMD2_valueChanged(double arg1)
{
  if (psPrintSettingsDMD2->PumpingMode == ON)
  {
    // divide by 1000 to scale from um to mm for stage input
    psPrintSettingsDMD2->PumpingParameter = arg1 / 1000;
  }
  else
  {
    emit SettingsPrint("Please enable pumping before setting pumping parameter");
  }
}

void printsettingsDMD2::on_StageVelocityParamDMD2_valueChanged(double arg1)
{
  psPrintSettingsDMD2->StageVelocity = arg1;
}

void printsettingsDMD2::on_StageAccelParamDMD2_valueChanged(double arg1)
{
  psPrintSettingsDMD2->StageAcceleration = arg1;
}

void printsettingsDMD2::on_MaxEndOfRunParamDMD2_valueChanged(double arg1)
{
  psPrintSettingsDMD2->MaxEndOfRun = arg1;
}

void printsettingsDMD2::on_MinEndOfRunParamDMD2_valueChanged(double arg1)
{
  psPrintSettingsDMD2->MinEndOfRun = arg1;
}

/***********************************Injection Control**********************************************/
void printsettingsDMD2::on_ContinuousInjectionDMD2_clicked()
{
  if (ui->ContinuousInjectionDMD2->isChecked() == true)
  {
    ui->SteppedContInjectionDMD2->setChecked(false);
    psInjectionSettingsDMD2->ContinuousInjection = ON;
    EnableParameter(BASE_INJECTION, ON);
    emit SettingsPrint("Continuouus injection selected");
  }
  else
  {
    psInjectionSettingsDMD2->ContinuousInjection = false;
    emit SettingsPrint("Continuous injection disabled");
  }
}

void printsettingsDMD2::on_SteppedContInjectionDMD2_clicked()
{
  if (ui->SteppedContInjectionDMD2->isChecked() == true)
  {
    ui->ContinuousInjectionDMD2->setChecked(false);
    psInjectionSettingsDMD2->SteppedContinuousInjection = ON;
    EnableParameter(BASE_INJECTION, ON);
  }
  else
  {
    psInjectionSettingsDMD2->SteppedContinuousInjection = false;
    emit SettingsPrint("Stepped continuous injection disabled");
  }
}

void printsettingsDMD2::on_BaseInfusionParamDMD2_valueChanged(double arg1)
{
  psInjectionSettingsDMD2->BaseInjectionRate = arg1;
}

void printsettingsDMD2::on_PreMovementCheckboxDMD2_clicked()
{
  if (ui->PreMovementCheckboxDMD2->isChecked() == true)
  {
    ui->PostMovementCheckboxDMD2->setChecked(false);
    psInjectionSettingsDMD2->InjectionDelayFlag = PRE;
    emit SettingsPrint("Pre-Injection delay enabled");
  }
  else
  {
    ui->PreMovementCheckboxDMD2->setChecked(false);
    psInjectionSettingsDMD2->InjectionDelayFlag = OFF;
    emit SettingsPrint("Injection delay disabled");
  }
}

void printsettingsDMD2::on_PostMovementCheckboxDMD2_clicked()
{
  if (ui->PostMovementCheckboxDMD2->isChecked() == true)
  {
    ui->PreMovementCheckboxDMD2->setChecked(false);
    psInjectionSettingsDMD2->InjectionDelayFlag = POST;
    emit SettingsPrint("Post-Injection delay enabled");
  }
  else
  {
    ui->PostMovementCheckboxDMD2->setChecked(false);
    psInjectionSettingsDMD2->InjectionDelayFlag = OFF;
    emit SettingsPrint("Injection delay disabled");
  }
}

void printsettingsDMD2::on_InjectionDelayParamDMD2_valueChanged(double arg1)
{
  psInjectionSettingsDMD2->InjectionDelayParam = arg1;
}

void printsettingsDMD2::on_InjectionRateParamDMD2_valueChanged(double arg1)
{
  psInjectionSettingsDMD2->InfusionRate = arg1;
}

void printsettingsDMD2::on_VolPerLayerParamDMD2_valueChanged(double arg1)
{
  psInjectionSettingsDMD2->InfusionVolume = arg1;
}

void printsettingsDMD2::on_InitialVolumeParamDMD2_valueChanged(double arg1)
{
  psInjectionSettingsDMD2->InitialVolume = arg1;
}

/***********************************Dynamic Print Mode*********************************************/
void printsettingsDMD2::on_UsePrintScriptDMD2_clicked()
{
  bool toggle;
  bool script;
  if (ui->UsePrintScriptDMD2->isChecked())
  {
    psPrintScriptDMD2->PrintScript = ON;
    script = ON;
    toggle = OFF;
  }
  else
  {
    psPrintScriptDMD2->PrintScript = OFF;
    script = OFF;
    toggle = ON;
  }
  ui->SelectPrintScriptDMD2->setEnabled(script);
  ui->ClearPrintScriptDMD2->setEnabled(script);
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

void printsettingsDMD2::on_SelectPrintScriptDMD2_clicked()
{
  QString file_name = QFileDialog::getOpenFileName(this, "Open Print Script", PrintScriptDirectory2, "*.txt *.csv");
  if (file_name != "")
  {
    initPrintScript(file_name);
  }
}

void printsettingsDMD2::initPrintScript(QString file_name)
{
  ui->PrintScriptFile->setText(file_name);
  QDir Directory = QFileInfo(file_name.at(0)).absoluteDir();   // Get directory selected
  PrintScriptDirectory2 = Directory.filePath(file_name.at(0)); // Save directory

  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly))
  {
    // qDebug() << file.errorString();
  }
  QStringList wordList;
  while (!file.atEnd())
  { // Runs until the end of the file
    QByteArray line = file.readLine();
    psPrintScriptDMD2->ExposureScriptList.append(line.split(',').at(0));
    psPrintScriptDMD2->LEDScriptList.append(line.split(',').at(1));
    psPrintScriptDMD2->DarkTimeScriptList.append(line.split(',').at(2));
    psPrintScriptDMD2->LayerThicknessScriptList.append(line.split(',').at(3));
    psPrintScriptDMD2->StageVelocityScriptList.append(line.split(',').at(4));
    psPrintScriptDMD2->StageAccelerationScriptList.append(line.split(',').at(5));
    psPrintScriptDMD2->PumpHeightScriptList.append(line.split(',').at(6));
    if (psPrintSettingsDMD2->PrinterType == ICLIP)
    {
      psPrintScriptDMD2->InjectionVolumeScriptList.append(line.split(',').at(7));
      psPrintScriptDMD2->InjectionRateScriptList.append(line.split(',').at(8));
    }
  }
  emit SettingsPrint("Print List has: " + QString::number(psPrintScriptDMD2->ExposureScriptList.size()) + " exposure time entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScriptDMD2->LEDScriptList.size()) + " LED intensity entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScriptDMD2->DarkTimeScriptList.size()) + " dark time entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScriptDMD2->LayerThicknessScriptList.size()) + " layer thickness entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScriptDMD2->StageVelocityScriptList.size()) + " stage velocity entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScriptDMD2->StageAccelerationScriptList.size()) + " stage acceleration entries");
  emit SettingsPrint("Print List has: " + QString::number(psPrintScriptDMD2->PumpHeightScriptList.size()) + +" pump height entries");

  emit updateScript();
}

void printsettingsDMD2::on_ClearPrintScriptDMD2_clicked()
{
  ui->PrintScriptFile->clear();
  psPrintScriptDMD2->ExposureScriptList.clear();
  psPrintScriptDMD2->LEDScriptList.clear();
  psPrintScriptDMD2->DarkTimeScriptList.clear();
  psPrintScriptDMD2->LayerThicknessScriptList.clear();
  psPrintScriptDMD2->StageVelocityScriptList.clear();
  psPrintScriptDMD2->StageAccelerationScriptList.clear();
  psPrintScriptDMD2->PumpHeightScriptList.clear();
  if (psPrintSettingsDMD2->PrinterType == ICLIP)
  {
    psPrintScriptDMD2->InjectionVolumeScriptList.clear();
    psPrintScriptDMD2->InjectionRateScriptList.clear();
  }
  emit updateScript();
}

/**********************************Image File Selection********************************************/
void printsettingsDMD2::on_SelectFileDMD2_clicked()
{
  // Open files from last directory chosen, limited to bitmapped or tiff image file formats
  QStringList file_names = QFileDialog::getOpenFileNames(this, "Open Object Image Files", ImageFileDirectory3, "*.bmp *.png *.tiff *.tif");
  initImageFilesDMD2(file_names);
}

void printsettingsDMD2::initImageFilesDMD2(QStringList file_names)
{
  if (file_names.count() > 0)
  {                                                                  // If images selected
    QDir ImageDirectory = QFileInfo(file_names.at(0)).absoluteDir(); // Get directory selected
    ImageFileDirectory3 = ImageDirectory.filePath(file_names.at(0)); // Save directory
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

void printsettingsDMD2::on_ClearImageFilesDMD2_clicked()
{
  ui->FileList->clear();
}

int printsettingsDMD2::FileListCount()
{
  return ui->FileList->count();
}

QString printsettingsDMD2::FileListItem(int itemNum)
{
  return ui->FileList->item(itemNum)->text();
}
/************************************Helper Functions**********************************************/
void printsettingsDMD2::EnableParameter(Parameter_t Parameter, bool State)
{
  switch (Parameter)
  {
  case EXPOSURE_TIME:
    ui->ExposureTimeParamDMD2->setEnabled(State);
    ui->ExposureTimeBoxDMD2->setEnabled(State);
    break;
  case LED_INTENSITY:
    ui->UVIntensityParamDMD2->setEnabled(State);
    ui->UVIntensityBoxDMD2->setEnabled(State);
    break;
  case DARK_TIME:
    ui->DarkTimeParamDMD2->setEnabled(State);
    ui->DarkTimeBoxDMD2->setEnabled(State);
    break;
  case LAYER_THICKNESS:
    ui->LayerThicknessParamDMD2->setEnabled(State);
    ui->LayerThicknessBoxDMD2->setEnabled(State);
    break;
  case STAGE_VELOCITY:
    ui->StageVelocityParamDMD2->setEnabled(State);
    ui->StageVelocityBoxDMD2->setEnabled(State);
    break;
  case STAGE_ACCELERATION:
    ui->StageAccelParamDMD2->setEnabled(State);
    ui->StageAccelerationBoxDMD2->setEnabled(State);
    break;
  case PUMP_HEIGHT:
    ui->pumpingParameterDMD2->setEnabled(State);
    break;
  case INJECTION_VOLUME:
    ui->VolPerLayerParamDMD2->setEnabled(State);
    ui->VolPerLayerBoxDMD2->setEnabled(State);
    break;
  case INJECTION_RATE:
    ui->InjectionRateParamDMD2->setEnabled(State);
    ui->InjectionRateBoxDMD2->setEnabled(State);
    break;
  case BASE_INJECTION:
    ui->BaseInfusionParamDMD2->setEnabled(State);
    ui->BaseInfusionRateTitleDMD2->setEnabled(State);
    break;
  case MAX_IMAGE:
    ui->MaxImageUploadDMD2->setEnabled(State);
    ui->MaxImageUploadBox->setEnabled(State);
    break;
  case VP_RESYNC:
    ui->ResyncRateListDMD2->setEnabled(State);
    ui->ResyncRateBox->setEnabled(State);
    break;
  case INITIAL_VOLUME:
    ui->InitialVolumeParamDMD2->setEnabled(State);
    ui->InitialVolumeBoxDMD2->setEnabled(State);
    break;
  case INITIAL_INTENSITY:
    ui->InitialExposureIntensityParamDMD2->setEnabled(State);
    ui->InitialIntensityBoxDMD2->setEnabled(State);
    break;
  case INITIAL_DELAY:
    ui->InitialDelayParamDMD2->setEnabled(State);
    ui->InitialDelayBoxDMD2->setEnabled(State);
    break;
  case CONTINUOUS_INJECTION:
    ui->ContinuousInjectionDMD2->setEnabled(State);
    ui->SteppedContInjectionDMD2->setEnabled(State);
    ui->ContInjectionBox->setEnabled(State);
    break;
  case STARTING_POSITION:
    ui->StartingPositionParamDMD2->setEnabled(State);
    ui->StartingPositionBox->setEnabled(State);
    break;
  case MAX_END:
    ui->MaxEndOfRunParamDMD2->setEnabled(State);
    ui->MaxEndOfRunBoxDMD2->setEnabled(State);
    break;
  case MIN_END:
    ui->MinEndOfRunParamDMD2->setEnabled(State);
    ui->MinEndOfRunBoxDMD2->setEnabled(State);
    break;
  case INJECTION_DELAY:
    ui->InjectionDelayParamDMD2->setEnabled(State);
    ui->InjectionDelayBoxDMD2->setEnabled(State);
    break;
  default:
    break;
  }
}

/**********************************Settings Functions**********************************************/
void printsettingsDMD2::savePrintSettings()
{
  QSettings settings;
  settings.setValue("Resin", psPrintSettingsDMD2->Resin);
  settings.setValue("BitMode", psPrintSettingsDMD2->BitMode);
  settings.setValue("MaxImageUpload", psPrintSettingsDMD2->MaxImageUpload);
  settings.setValue("ResyncVP", psPrintSettingsDMD2->ResyncVP);
  settings.setValue("LayerThickness", psPrintSettingsDMD2->LayerThickness);
  settings.setValue("StartingPosition", psPrintSettingsDMD2->StartingPosition);
  settings.setValue("PostExposureDelay", psPrintSettingsDMD2->PostExposureDelay);
  settings.setValue("JerkTime", psPrintSettingsDMD2->JerkTime);

  settings.setValue("ProjectionMode", psPrintSettingsDMD2->ProjectionMode);
  settings.setValue("InitialExposure", psPrintSettingsDMD2->InitialExposure);
  settings.setValue("InitialDelay", psPrintSettingsDMD2->InitialDelay);
  settings.setValue("InitialIntensity", psPrintSettingsDMD2->InitialIntensity);
  settings.setValue("UVIntensity", psPrintSettingsDMD2->UVIntensity);
  settings.setValue("ExposureTime", psPrintSettingsDMD2->ExposureTime);
  settings.setValue("DarkTime", psPrintSettingsDMD2->DarkTime);

  settings.setValue("MotionMode", psPrintSettingsDMD2->MotionMode);
  settings.setValue("PumpingMode", psPrintSettingsDMD2->PumpingMode);
  settings.setValue("PumpingParameter", psPrintSettingsDMD2->PumpingParameter);
  settings.setValue("StageVelocity", psPrintSettingsDMD2->StageVelocity);
  settings.setValue("StageAcceleration", psPrintSettingsDMD2->StageAcceleration);
  settings.setValue("MaxEndOfRun", psPrintSettingsDMD2->MaxEndOfRun);
  settings.setValue("MinEndOfRun", psPrintSettingsDMD2->MinEndOfRun);

  settings.setValue("InfusionRate", psInjectionSettingsDMD2->InfusionRate);
  settings.setValue("InfusionVolume", psInjectionSettingsDMD2->InfusionVolume);
  settings.setValue("InitialVolume", psInjectionSettingsDMD2->InfusionVolume);
  settings.setValue("BaseInjectionRate", psInjectionSettingsDMD2->BaseInjectionRate);
  settings.setValue("InjectionDelay", psInjectionSettingsDMD2->InjectionDelayParam);

  settings.setValue("PrintScriptActive", psPrintScriptDMD2->PrintScript);
  settings.setValue("PrintScript", ui->PrintScriptFile->text());
  settings.setValue("PrintScriptDirectory2", PrintScriptDirectory2);
  settings.setValue("ImageFileDirectory", ImageFileDirectory3);
  QStringList ImageList;
  QListWidgetItem *item;
  for (int i = 0; i < ui->FileList->count(); i++)
  {
    item = ui->FileList->item(i);
    ImageList << item->text();
  }
  settings.setValue("ImageFiles", ImageList);
}

void printsettingsDMD2::loadPrintSettings()
{
  QSettings settings;
  psPrintSettingsDMD2->Resin = settings.value("Resin", "No resin selected").toString();
  psPrintSettingsDMD2->BitMode = settings.value("BitMode", 1).toDouble();
  psPrintSettingsDMD2->MaxImageUpload = settings.value("MaxImageUpload", 50).toDouble();
  psPrintSettingsDMD2->ResyncVP = settings.value("ResyncVP", 24).toDouble();
  psPrintSettingsDMD2->LayerThickness = settings.value("LayerThickness", 1).toDouble();
  psPrintSettingsDMD2->StartingPosition = settings.value("StartingPosition", 5).toDouble();
  psPrintSettingsDMD2->PostExposureDelay = settings.value("PostExposureDelay", 0).toDouble();
  psPrintSettingsDMD2->JerkTime = settings.value("JerkTime", 0.04).toDouble();

  psPrintSettingsDMD2->ProjectionMode = settings.value("ProjectionMode", POTF).toInt();
  psPrintSettingsDMD2->InitialExposure = settings.value("InitialExposure", 10).toDouble();
  psPrintSettingsDMD2->InitialDelay = settings.value("InitialDelay", 0).toInt();
  psPrintSettingsDMD2->InitialIntensity = settings.value("InitialIntensity", 10).toInt();
  psPrintSettingsDMD2->UVIntensity = settings.value("UVIntensity", 12).toDouble();
  psPrintSettingsDMD2->ExposureTime = settings.value("ExposureTime", 1000).toDouble();
  psPrintSettingsDMD2->DarkTime = settings.value("DarkTime", 1000).toDouble();

  psPrintSettingsDMD2->MotionMode = settings.value("MotionMode", STEPPED).toDouble();
  psPrintSettingsDMD2->PumpingMode = settings.value("PumpingMode", 0).toDouble();
  psPrintSettingsDMD2->PumpingParameter = settings.value("PumpingParameter", 0).toDouble();
  psPrintSettingsDMD2->StageVelocity = settings.value("StageVelocity", 10).toDouble();
  psPrintSettingsDMD2->StageAcceleration = settings.value("StageAcceleration", 5).toDouble();
  psPrintSettingsDMD2->MaxEndOfRun = settings.value("MaxEndOfRun", 60).toDouble();
  psPrintSettingsDMD2->MinEndOfRun = settings.value("MinEndOfRun", 0).toDouble();

  psInjectionSettingsDMD2->InfusionRate = settings.value("InfusionRate", 5).toDouble();
  psInjectionSettingsDMD2->InfusionVolume = settings.value("InfusionVolume", 5).toDouble();
  psInjectionSettingsDMD2->InitialVolume = settings.value("InitialVolume", 0).toDouble();
  psInjectionSettingsDMD2->BaseInjectionRate = settings.value("BaseInjectionRate", 0).toDouble();
  psInjectionSettingsDMD2->InjectionDelayParam = settings.value("InjectionDelay", 0).toDouble();

  ImageFileDirectory3 = settings.value("ImageFileDirectory", "C://").toString();
  PrintScriptDirectory2 = settings.value("PrintScriptDirectory2", "C://").toString();
  psPrintScriptDMD2->PrintScript = settings.value("PrintScriptActive", 0).toInt();

  // These are initialized here because they are not stored in variables only ui
  QString PrintScriptFile = settings.value("PrintScript", "").toString();
  if (PrintScriptFile != "" && psPrintScriptDMD2->PrintScript == ON)
  {
    initPrintScript(PrintScriptFile);
  }
  QStringList ImageFiles = settings.value("ImageFiles", "").toStringList();
  if (ImageFiles.isEmpty() == false)
  { // Needs to be nested to avoid segmentation fault
    if (ImageFiles.at(0) != "")
    {
      initImageFilesDMD2(ImageFiles);
    }
  }
}

void printsettingsDMD2::logCurrentSettingsToFile()
{
  QFile file("C:/Users/HOME-PC/Documents/CLIP3Dv2/settings_logDMD2.txt");
  if (file.open(QIODevice::WriteOnly | QIODevice::Append))
  {
    QTextStream out(&file);
    out << "DMD 2 Resin: " << psPrintSettingsDMD2->Resin << "\n";
    out << "DMD 2 BitMode: " << psPrintSettingsDMD2->BitMode << "\n";
    out << "DMD 2 MaxImageUpload: " << psPrintSettingsDMD2->MaxImageUpload << "\n";
    out << "DMD 2 ResyncVP: " << psPrintSettingsDMD2->ResyncVP << "\n";
    out << "DMD 2 LayerThickness: " << psPrintSettingsDMD2->LayerThickness << "\n";
    out << "DMD 2 StartingPosition: " << psPrintSettingsDMD2->StartingPosition << "\n";
    out << "DMD 2 PostExposureDelay: " << psPrintSettingsDMD2->PostExposureDelay << "\n";
    out << "DMD 2 JerkTime: " << psPrintSettingsDMD2->JerkTime << "\n";
    out << "DMD 2 InitialExposure: " << psPrintSettingsDMD2->InitialExposure << "\n";
    out << "DMD 2 InitialDelay: " << psPrintSettingsDMD2->InitialDelay << "\n";
    out << "DMD 2 InitialIntensity: " << psPrintSettingsDMD2->InitialIntensity << "\n";
    out << "DMD 2 UVIntensity: " << psPrintSettingsDMD2->UVIntensity << "\n";
    out << "DMD 2 ExposureTime: " << psPrintSettingsDMD2->ExposureTime << "\n";
    out << "DMD 2 DarkTime: " << psPrintSettingsDMD2->DarkTime << "\n";
    out << "DMD 2 MotionMode: " << psPrintSettingsDMD2->MotionMode << "\n";
    out << "DMD 2 PumpingMode: " << psPrintSettingsDMD2->PumpingMode << "\n";
    out << "DMD 2 StageVelocity: " << psPrintSettingsDMD2->StageVelocity << "\n";
    out << "DMD 2 StageAcceleration: " << psPrintSettingsDMD2->StageAcceleration << "\n";
    out << "DMD 2 MaxEndOfRun: " << psPrintSettingsDMD2->MaxEndOfRun << "\n";
    out << "DMD 2 MinEndOfRun: " << psPrintSettingsDMD2->MinEndOfRun << "\n";
    out << "DMD 2 InfusionRate: " << psInjectionSettingsDMD2->InfusionRate << "\n";
    out << "DMD 2 InfusionVolume: " << psInjectionSettingsDMD2->InfusionVolume << "\n";
    out << "DMD 2 InitialVolume: " << psInjectionSettingsDMD2->InitialVolume << "\n";
    out << "DMD 2 BaseInjectionRate: " << psInjectionSettingsDMD2->BaseInjectionRate << "\n";
    out << "DMD 2 InjectionDelayParam: " << psInjectionSettingsDMD2->InjectionDelayParam << "\n";
    out << "DMD 2 PrintScript: " << psPrintScriptDMD2->PrintScript << "\n";
    file.close();
  }
}

void printsettingsDMD2::initPrintSettings()
{
  // init resin
  bool FoundMatch = false;
  QStringList itemsInComboBox;
  for (int index = 0; index < ui->resinSelectDMD2->count(); index++)
  {
    if (psPrintSettingsDMD2->Resin == ui->resinSelectDMD2->itemText(index))
    {
      ui->resinSelectDMD2->setCurrentIndex(index);
      FoundMatch = true;
      break;
    }
  }
  if (FoundMatch == false)
  {
    ui->resinSelectDMD2->addItem(psPrintSettingsDMD2->Resin);
    ui->resinSelectDMD2->setCurrentIndex(ui->resinSelectDMD2->count() - 1);
  }
  ui->BitDepthParamDMD2->setValue(psPrintSettingsDMD2->BitMode);
  ui->MaxImageUploadDMD2->setValue(psPrintSettingsDMD2->MaxImageUpload);
  ui->ResyncRateListDMD2->setCurrentIndex((psPrintSettingsDMD2->ResyncVP / 24) - 1);
  ui->LayerThicknessParamDMD2->setValue(psPrintSettingsDMD2->LayerThickness * 1000);
  ui->StartingPositionParamDMD2->setValue(psPrintSettingsDMD2->StartingPosition);
  ui->PostExposureDelayParamDMD2->setValue(psPrintSettingsDMD2->PostExposureDelay);
  ui->JerkTimeParamDMD2->setValue(psPrintSettingsDMD2->JerkTime * 1000); // convert from s to ms

  ui->InitialExposureParameterDMD2->setValue(psPrintSettingsDMD2->InitialExposure);
  ui->InitialDelayParamDMD2->setValue(psPrintSettingsDMD2->InitialDelay);
  ui->InitialExposureIntensityParamDMD2->setValue(psPrintSettingsDMD2->InitialIntensity);
  ui->UVIntensityParamDMD2->setValue(psPrintSettingsDMD2->UVIntensity);
  ui->ExposureTimeParamDMD2->setValue(psPrintSettingsDMD2->ExposureTime / 1000);
  ui->DarkTimeParamDMD2->setValue(psPrintSettingsDMD2->DarkTime / 1000);

  if (psPrintSettingsDMD2->MotionMode == STEPPED)
  {
    ui->SteppedMotionButtonDMD2->setChecked(true);
    ui->ContinuousMotionButtonDMD2->setChecked(false);
    emit(on_SteppedMotionButtonDMD2_clicked());
  }
  else if (psPrintSettingsDMD2->MotionMode == CONTINUOUS)
  {
    ui->ContinuousMotionButtonDMD2->setChecked(true);
    ui->SteppedMotionButtonDMD2->setChecked(false);
    emit(on_ContinuousMotionButtonDMD2_clicked());
  }

  if (psPrintSettingsDMD2->PumpingMode == ON)
  {
    ui->pumpingCheckBoxDMD2->setChecked(true);
    emit(on_pumpingCheckBoxDMD2_clicked());
  }
  else if (psPrintSettingsDMD2->PumpingMode == OFF)
  {
    ui->pumpingCheckBoxDMD2->setChecked(false);
  }
  ui->StageVelocityParamDMD2->setValue(psPrintSettingsDMD2->StageVelocity);
  ui->StageAccelParamDMD2->setValue(psPrintSettingsDMD2->StageAcceleration);
  ui->MaxEndOfRunParamDMD2->setValue(psPrintSettingsDMD2->MaxEndOfRun);
  ui->MinEndOfRunParamDMD2->setValue(psPrintSettingsDMD2->MinEndOfRun);

  ui->InjectionRateParamDMD2->setValue(psInjectionSettingsDMD2->InfusionRate);
  ui->VolPerLayerParamDMD2->setValue(psInjectionSettingsDMD2->InfusionVolume);
  ui->InitialVolumeParamDMD2->setValue(psInjectionSettingsDMD2->InfusionVolume);
  ui->BaseInfusionParamDMD2->setValue(psInjectionSettingsDMD2->BaseInjectionRate);
  ui->InjectionDelayParamDMD2->setValue(psInjectionSettingsDMD2->InjectionDelayParam);

  ui->UsePrintScriptDMD2->setChecked(psPrintScriptDMD2->PrintScript);
  if (psPrintScriptDMD2->PrintScript == ON)
  {
    on_UsePrintScriptDMD2_clicked(); // turn on print script
  }
  EnableParameter(VP_RESYNC, OFF);     // Always starts in POTF so ResyncVP is disabled from start
  EnableParameter(INITIAL_DELAY, OFF); // Always starts in POTF so Initial delay is disabled

  logCurrentSettingsToFile();
}

#if 0

#endif
