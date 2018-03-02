#include <QtCore/QtCore>
#include "global.hpp"
#include "filemanager.hpp"
#include "tabsmodel.hpp"
#include "webpage.hpp"
#include "searchdb.hpp"
#include "webpage.hpp"
#include "keymaps.hpp"
#include "url.hpp"
#include "crawler.hpp"

#ifdef Q_OS_MACOS
#include "mac_crawler.hpp"
#endif

void dumpLibraryInfo()
{
    qCInfo(GlobalLogging) << "QLibraryInfo::PrefixPath" << QLibraryInfo::location(QLibraryInfo::PrefixPath);
    qCInfo(GlobalLogging) << "QLibraryInfo::LibrariesPath" << QLibraryInfo::location(QLibraryInfo::LibrariesPath);
    qCInfo(GlobalLogging) << "QLibraryInfo::PluginsPath" << QLibraryInfo::location(QLibraryInfo::PluginsPath);
    qCInfo(GlobalLogging) << "QCoreApplication::applicationDirPath" << QCoreApplication::applicationDirPath();
}

Global::Global(QObject *parent) : QObject(parent)
{
}

void Global::startQCoreApplicationThread(int argc, const QStringList& argv) {
    static std::vector<char*> vec(argc);
    static QList<QByteArray> bytes;
    bytes.reserve(argc);
    for (int i = 0; i < argc; i++) {
        bytes << argv[i].toLatin1();
        vec[i] = bytes[i].data();
    }

    Global::startQCoreApplicationThread(argc, vec.data());
}

void Global::startQCoreApplicationThread(int argc, char** argv) {
    if (qCoreApplication) { return; } // already running

    qRegisterMetaType<void const*>();
    qRegisterMetaType<CrawlerRule>();
    qRegisterMetaType<CrawlerRuleTable>();
    qRegisterMetaType<uint_least64_t>();

    qCoreApplication = new QCoreApplication(argc, argv);
    qCoreApplication->moveToThread(qCoreApplicationThread);

    QObject::connect(qCoreApplicationThread, &QThread::started, [=]() {
        dumpLibraryInfo();

        QString currV = FileManager::readQrcFileS("defaults/version");
        QString dataV = FileManager::readDataFileS("version");
        qCCritical(GlobalLogging) << "running version" << currV << "data version" << dataV;
        if (currV != dataV) {
            Global::isNewInstall = true;
            FileManager::rmDataDir();
            FileManager::mkDataDir();
        }
        initGlobalObjects();
        Global::sig.emit_tf_global_objects_initialized();
        searchDB->connect();
        controller->loadLastOpen();
        controller->loadBookmarks();
        Global::sig.emit_tf_everything_loaded();
    });

    QObject::connect(qCoreApplicationThread, &QThread::finished, [=]() {
        searchDB->disconnect();
    });

    qCoreApplicationThread->start();
}

void Global::stopQCoreApplicationThread()
{
    qCoreApplicationThread->quit();
    qCoreApplicationThread->wait();
}

QCoreApplication* Global::qCoreApplication = nullptr;
QThread* const Global::qCoreApplicationThread = new QThread();

GlobalSignals Global::sig;
//FileManager* Global::fileManager = new FileManager();
SearchDB* Global::searchDB = nullptr;
Controller* Global::controller = nullptr;
Crawler* Global::crawler = nullptr;
//KeyMaps* Global::keyMaps = new KeyMaps();
//SettingsModel* Global::settingsModel = new SettingsModel();

void Global::initGlobalObjects()
{
    Q_ASSERT(QThread::currentThread() == qCoreApplicationThread);
    searchDB = new SearchDB();
    controller = new Controller();

#ifdef Q_OS_MACOS
    crawler = new Crawler(MacCrawlerDelegateFactory_::create(), 100, 0);
#endif

}

bool Global::isNewInstall = false;
