const electron = require('electron');
const app = electron.app;

const BrowserWindow = electron.BrowserWindow;

let mainWindow;

function opened () {
/*  childWindow = new BrowserWindow ({width: 1800,
                                    height: 1200,
                                    title: 'medi'});

  childWindow.loadURL (`file://${__dirname}/main.html`);*/
  mainWindow.close();
}

function closed () {
}

function received () {
}

function startClient () {
  const socket = new WebSocket("ws://localhost:2015");
  socket.onmessage = received;
  socket.onopen    = opened;
  socket.onclose   = closed;
}

function createWindow () {

  mainWindow = new BrowserWindow({width: 1800,
                                  height: 1200,
                                  title: 'splash',
                                  center: true,
                                  frame: false,
                                  transparent: true
                                 });

  mainWindow.loadURL(`file://${__dirname}/index.html`);

  mainWindow.on('closed', () => {
    mainWindow = null;
  })
}

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
