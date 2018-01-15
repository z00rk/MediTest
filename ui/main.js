const electron = require('electron');
const app = electron.app;

const BrowserWindow = electron.BrowserWindow;

let mainWindow;

function startSocket () {
  const socket = new WebSocket ("ws://127.0.0.1:2015");
  socket.onopen = function () {
    /**
     * this won't work unless we update electron -- @TODO later
     * we will use a BrowserWindowProxy for now until we
     * figure out how we will deal with it
     *
     * mainWindow.loadURL(`file://${__dirname}/layout.html`);
     **/
    setTimeout (window.open (`file://${__dirname}/layout.html`), 1000);
//    mainWindow.hide ();
  }
};

function createWindow () {

  mainWindow = new BrowserWindow({width: 1800,
                                  height: 1200,
                                  title: 'splash',
                                  center: true,
                                  frame: false,
                                  transparent: true,
                                  show: false,
                                  fullscreen: true
                                 });

  //avoid flashing when app is launched
  mainWindow.once('ready-to-show', () => {
    mainWindow.show()
  })

  mainWindow.loadURL(`file://${__dirname}/splash.html`);

  mainWindow.on('closed', () => {
    mainWindow = null;
  })
};

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});
