#define FILE_FILTER "Blended animation files (*.avbl)"
#define AVM_FILTER "Animation files (*.avm)"


#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QStringListModel>

#include "Avbl.h"
#include "BlenderTab.h"
#include "ui_BlenderTab.h"
#include "qavimator.h"
#include "animationview.h"
#include "settings.h"
#include "OptionalMessageBox.h"


BlenderTab::BlenderTab(qavimator* mainWindow, const QString& fileName, bool createFile)
  : QWidget(0), AbstractDocumentTab(mainWindow)
{
  setupUi(this);
  canShowWarn = false;

  setAttribute(Qt::WA_DeleteOnClose);

  connect(this, SIGNAL(resetCamera()), blenderAnimationView, SLOT(resetCamera()));
  connect(blenderTimeline, SIGNAL(resultingAnimationChanged(WeightedAnimation*)), blenderPlayer, SLOT(onAnimationChanged(WeightedAnimation*)));
  connect(blenderTimeline, SIGNAL(resultingAnimationChanged(WeightedAnimation*)), this, SLOT(onTimelineAnimationChanged(WeightedAnimation*)));

  if(createFile)
  {
    CurrentFile = fileName;
    fileNew();
    Save();
  }
  else
    fileOpen(fileName);

  bindMenuActions();
}

BlenderTab::~BlenderTab()
{
//  delete ui;
}


void BlenderTab::bindMenuActions()
{
/*  connect(mainWindow->optionsLoopAction, SIGNAL(toggled(bool)), this, SLOT(optionsLoopAction_toggled(bool)));
  connect(mainWindow->optionsProtectFirstFrameAction, SIGNAL(triggered(bool)), this, SLOT(optionsProtectFirstFrameAction_toggled(bool)));   */
}

bool BlenderTab::IsUnsaved()
{
  //TODO
  return true;
}

void BlenderTab::AddFile()
{
  animsList->addNewFile();
}

void BlenderTab::Save()
{
  sorry();
}

void BlenderTab::SaveAs()
{
  fileSaveAs();
}

void BlenderTab::Cut()
{
  sorry();
}

void BlenderTab::Copy()
{
  sorry();
}

void BlenderTab::Paste()
{
  sorry();
}

void BlenderTab::ResetView()
{
  emit resetCamera();
}

void BlenderTab::ExportForSecondLife()
{
  sorry();
}

void BlenderTab::UpdateToolbar()
{
  mainWindow->resetCameraAction->setVisible(true);
  mainWindow->resetCameraAction->setEnabled(true);
  mainWindow->fileAddAction->setEnabled(true);
  mainWindow->editCutAction->setVisible(false);
}

void BlenderTab::UpdateMenu()
{
  //Note: actions shared with Toolbar are handled in UpdateToolbar()
  mainWindow->fileLoadPropsAction->setEnabled(false);
  mainWindow->fileSavePropsAction->setEnabled(false);

  mainWindow->toolsOptimizeBVHAction->setEnabled(false);
  mainWindow->toolsMirrorAction->setEnabled(false);

  mainWindow->optionsJointLimitsAction->setEnabled(false);
  mainWindow->optionsLoopAction->setEnabled(true);
  mainWindow->optionsProtectFirstFrameAction->setEnabled(false);
  mainWindow->optionsShowTimelineAction->setEnabled(false);
  mainWindow->optionsSkeletonAction->setEnabled(false);
}

void BlenderTab::onTabActivated()
{
  // hack to get 3D view back in shape
//  qApp->processEvents();
  blenderAnimationView->makeCurrent();
  QSize oldSize = blenderAnimationView->size();
  blenderAnimationView->resize(oldSize.width(), oldSize.height()-1);
  qApp->processEvents();
  blenderAnimationView->resize(oldSize);

  UpdateToolbar();
  UpdateMenu();
}

// convenience function to set window title in a defined way
void BlenderTab::setCurrentFile(const QString& fileName)
{
  CurrentFile=fileName;
  mainWindow->setWindowTitle("Animik ["+CurrentFile+"]");       //edu: Fuj! TODO: encapsulate.
  QFileInfo fileInfo(fileName);
  setWindowTitle(fileInfo.fileName());
}

// Menu action: File / Open ...
void BlenderTab::fileOpen()
{
  fileOpen(QString::null);
}

void BlenderTab::fileOpen(const QString& name)
{
  fileAdd(name);
}

void BlenderTab::fileAdd()
{
  fileAdd(QString::null);
}

QString BlenderTab::selectFileToOpen(const QString& caption)        //TODO: resolve the ugly code multiplication
{
   //// For some unknown reason passing "this" locks up the OSX qavimator window. Possibly a QT4 bug, needs investigation
#ifdef __APPLE__
  QString file=QFileDialog::getOpenFileName(NULL,caption, Settings::Instance()->lastPath(), FILE_FILTER);
#else
   QString file=QFileDialog::getOpenFileName(this,caption,Settings::Instance()->lastPath(), FILE_FILTER);
#endif
  if(!file.isEmpty())
  {
    QFileInfo fileInfo(file);
    if(!fileInfo.exists())
    {
      QMessageBox::warning(this,QObject::tr("Load Animation File"),
                           QObject::tr("<qt>Animation file not found:<br />%1</qt>").arg(file));
      file=QString::null;
    }
    else
      Settings::Instance()->setLastPath(fileInfo.path());
  }

  return file;
}

void BlenderTab::fileAdd(const QString& name)
{
  QString file=name;

  if(file.isEmpty())
    file=selectFileToOpen(tr("Select Animation File to Add"));

  if(!file.isEmpty())
  {
    // handling of non-existant file names
    if(!QFile::exists(file))
    {
      QMessageBox::warning(this, QObject::tr("Load Animation File"),
                           QObject::tr("<qt>Animation file not found:<br />%1</qt>").arg(file));
      return;
    }

    setCurrentFile(file);

/*TODO    if(avblFile->getLoopInPoint()==-1)
    {
      // first set loop out point to avoid clamping of loop in point
      setLoopOutPoint(anim->getNumberOfFrames());

      if(protectFirstFrame)
      {
//        qDebug("qavimator::fileAdd(): adding loop points for protected frame 1 animation");
        anim->setFrame(1);
        setCurrentFrame(1);
        setLoopInPoint(2);
      }
      else
      {
//        qDebug("qavimator::fileAdd(): adding loop points for unprotected frame 1 animation");
        blenderTimeline->setFrame(0);
        setCurrentFrame(0);
        setLoopInPoint(1);
      }
    }
    else
    {
//      qDebug("qavimator::fileAdd(): reading saved loop points");
      setLoopInPoint(anim->getLoopInPoint()+1);
      setLoopOutPoint(anim->getLoopOutPoint()+1);
    }     */

//TODO?    connect(anim,SIGNAL(currentFrame(int)),this,SLOT(setCurrentFrame(int)));
  }
}


// Menu Action: File / Save As...
void BlenderTab::fileSaveAs()         //Ugly code repetition. TODO: think of it (I mean act)
{
   //// For some unknown reason passing "this" locks up the OSX qavimator window. Possibly a QT4 bug, needs investigation
#ifdef __APPLE__
   QString file=QFileDialog::getSaveFileName(NULL, tr("Save Composition File"), CurrentFile,
                                             "Composition Files (*.avbl)", 0,
                                             QFileDialog::DontConfirmOverwrite);
#else
   QString file=QFileDialog::getSaveFileName(this, tr("Save Composition File"), CurrentFile,
                                             "Composition Files (*.avbl)", 0,
                                             QFileDialog::DontConfirmOverwrite);
#endif

  if(!file.isEmpty())
  {
    QFileInfo fileInfo(file);

    // make sure file has proper extension (either .bvh or .avm)
    QString extension=fileInfo.suffix().toLower();
    if(extension!="avbl")
      file+=".avbl";

    // if the file didn't exist yet or the user accepted to overwrite it, save it.
    if(checkFileOverwrite(fileInfo))
    {
      setCurrentFile(file);
      Settings::Instance()->setLastPath(fileInfo.path());

      Avbl* saver = new Avbl();
      bool saved = saver->SaveToFile(blenderTimeline->Trails(), file);

      mainWindow->fileExportForSecondLifeAction->setEnabled(true);      //TODO: why?
    }
  }
}


// checks if a file already exists at the given path and displays a warning message
// returns true if it's ok to save/overwrite, else returns false
bool BlenderTab::checkFileOverwrite(const QFileInfo& fileInfo)      //VERY UGLY CODE CLONE (FROM KeyFramerTab). TODO: parent class? Or some FileHelper?
{
  // get file info
  if(fileInfo.exists())
  {
    int answer=QMessageBox::question(this,tr("File Exists"),tr("A file with the name \"%1\" does already exist. Do you want to overwrite it?").arg(fileInfo.fileName()),QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton);
    if(answer==QMessageBox::No) return false;
  }
  return true;
}


// prevent closing of main window if there are unsaved changes
void BlenderTab::closeEvent(QCloseEvent* event)
{
  if(!resolveUnsavedChanges())
    event->ignore();
  else
    event->accept();
}

bool BlenderTab::resolveUnsavedChanges()
{
  if(IsUnsaved())
  {
    QMessageBox saveMessage;
    saveMessage.setText(tr("The file %1 has been modified. Do you want to save it?").arg(CurrentFile));
    saveMessage.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    saveMessage.setDefaultButton(QMessageBox::Save);
    int answer = saveMessage.exec();
    if(answer==QMessageBox::Save)
      Save();
    else if(answer==QMessageBox::Cancel)
      return false;
  }

  return true;
}

void BlenderTab::fileNew()
{
  //TODO: open empty scene and timeline
}


// ------- Autoconnection of UI elements -------- //

void BlenderTab::on_animsList_AnimationFileTaken(QString filename, int orderInBatch, int batchSize)
{
  WeightedAnimation* anim = new WeightedAnimation(blenderAnimationView->getBVH(), filename);
  if(anim->isFirstFrameTPose())
    canShowWarn = true;

  QFileInfo fInfo(filename);
  if(!blenderTimeline->AddAnimation(anim, fInfo.completeBaseName()))
    QMessageBox::warning(this, "Error loading animation", "Can't add animation file '" +
                         filename + "' to timeline. Not enough space.");

  if(canShowWarn && orderInBatch==batchSize && Settings::Instance()->tPoseWarning())
  {
    QString* message = new QString("Animation you are adding to the time-line has initial posture of T-pose.\r\n");
    message->append("This is useful for Second Life animations but may be undesirable when joining multiple animations. ");
    message->append("To adjust such animation, select 'Cut initial T-pose frame' from it's context menu.");
    OptionalMessageBox* omb = new OptionalMessageBox("Initial T-pose", *message, this);
    omb->exec();
    Settings::Instance()->setTPoseWarning(omb->showAgain());
    canShowWarn = false;
  }
}

void BlenderTab::on_zoomInButton_clicked()
{
  //TODO
}

void BlenderTab::on_zoomOutButton_clicked()
{
  //TODO
}

// ---------------------------------------------- //

// --------------- other slots ------------------ //
void BlenderTab::onTimelineAnimationChanged(WeightedAnimation* anim)
{
  blenderAnimationView->setAnimation(anim);
}

// ---------------------------------------------- //




////////////////////////////////////////////// DEBUG //////////////////////////////////////////////

void BlenderTab::sorry()
{
  QMessageBox::warning(this, "Under construction", "Not implemented yet. Sorry.");
}
