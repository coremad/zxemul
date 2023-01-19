document.addEventListener("DOMContentLoaded", () => {

const DEBUG = 1;

const interval = 20;
const width  = 640;
const height  = 480;

const status = document.getElementById('status');
const canvas = document.getElementById('canvas');
const debug = document.getElementById('debug');
const debug2 = document.getElementById('debug2');
const html_fps = document.getElementById('fps');
const pbutton = document.getElementById('pbutton');
const cbutton = document.getElementById('cbutton');
const rlbutton = document.getElementById('rlbutton');
const ifile = document.getElementById('file-input');
const islider = document.getElementById("iRange");
const itacts = document.getElementById("itacts");
const czx = document.getElementById("zx");
const mbut = document.getElementById("dropbtn");
const build = document.getElementById("build");
//const tape_button = document.getElementById("tape_button")


const ctx = canvas.getContext('2d');

mbut.onclick = function() {
    document.getElementById("myDropdown").classList.toggle("show");
}

function toHex(d, n = 4) {
    return (d+0x10000).toString(16).substr(-n).toUpperCase();
}

var showregs = 0;
var showregsA;
var zx;

const maxStr = 60;
var str = 0;


ctx.canvas.width  = width;
ctx.canvas.height  = height;
const importObject = { imports: { imported_func: arg => console.log(arg) } };

    if (!window.AudioContext) {
        if (!window.webkitAudioContext) {
            alert("Your browser does not support any AudioContext and cannot play back this audio.");
            return;
        }
        window.AudioContext = window.webkitAudioContext;
    }
    var actx = new AudioContext({
      sampleRate: 48000,
      latencyHint: "interactive",
    });

fetch("js/undead-zx.wasm?seed=" + Math.random())
  .then((response) => response.arrayBuffer())
  .then((bytes) => WebAssembly.instantiate(bytes))
  .then((results) => {
    console.log(results.instance);
    zx = new TZX(results.instance, width, height);
    build.innerHTML = "build: " +
            String.fromCharCode.apply(null, zx.bDate) +
            " " +
            String.fromCharCode.apply(null, zx.bTime);
//    zx.init();

    pbutton.onclick = pause;
    cbutton.onclick = reset;

    islider.value = zx.itacts[0];
    itacts.innerHTML = ("000000" + zx.itacts[0]).slice(-6);
    islider.oninput = function() {
        zx.itacts[0] = this.value;
        itacts.innerHTML = ("000000" + this.value).slice(-6);
    }

    ifile.type = 'file';
    ifile.onchange = e => {
       var file = e.target.files[0];
       var reader = new FileReader();
       reader.readAsArrayBuffer(file);
       reader.onload = readerEvent => {
            pause();
            var buf = readerEvent.target.result;
            var sna = new Uint8Array(buf);
            while (zx.emul_active) console.log("active");
            for (var i = 0; i< buf.byteLength; i++) zx.zxmem[i + 16384] = sna[i + 27];
            for (var i = 0; i <  27; i++) zx.sna[i] = sna[i];
            zx.initSNA48k();
            ifile.value = '';
            str = 0; debug.innerHTML = "";
            pause();
       }
    }
    lbutton.onclick = function() { ifile.click(); };

    sbutton.onclick = saveSNA;
    function saveSNA() {
        pause();
        zx.dumpSNA48k();
        var buf = new ArrayBuffer(48*1024+27);
        var sna = new Uint8Array(buf);
        while (zx.emul_active) console.log("active");
        for (var i = 0; i< buf.byteLength; i++) sna[i + 27] = zx.zxmem[i + 16384];
        for (var i = 0; i <  27; i++) sna[i] = zx.sna[i];
        var blob = new Blob([sna], {type: "application/octet-stream"});
        saveAs(blob, "snapshot.sna");
        pause();
    };

    rlbutton.onclick = function() { str = 0; debug.innerHTML = ''; };
        canvas.addEventListener('click', event => pick(event));
    window.addEventListener('keydown', event => checkKeys(event), false);
    window.addEventListener('keyup', event => checkKeys(event), false);

    let elements = document.getElementsByClassName("fetch_sna");
    for(let i = 0; i < elements.length; i++) elements[i].onclick = function (e) {
            pause();
            zx.loadSNA('roms/' + e.srcElement.innerHTML + '.sna');
            str = 0; debug.innerHTML = "";
            pause();
        }

    tape_button.onclick = function(e) {
        pause();
        console.log('tape');
        loadTAPE("roms/BEYOND4K.TAP");
        zx.startTape();
        console.log(zx.tapeStarted[0]);
        pause();
    }

    async function loadTAPE(tape) {
        var resp = await fetch(tape);
        var blob = await resp.blob();
        var buf = await blob.arrayBuffer();
        var tape = new Uint8Array(buf);
        var len = Math.min(buf.byteLength, 256*1024);
        zx.tapeSize[0] = len;
        for (var i = 0; i < len; i++) zx.tape[i] = tape[i];
    }

    elements = document.getElementsByClassName("model");
    for(let i = 0; i < elements.length; i++) elements[i].onclick = function (e) {
            zx.itacts[0] = e.target.getAttribute("data-t");
            islider.value = zx.itacts[0];
            itacts.innerHTML = ("000000" + zx.itacts[0]).slice(-6);
        }

    const fkempston = { kLeft:1, kRigth:2, kUp:4, kDown:8, kFire:16 };
    var start_button = 0;
    window.addEventListener('gamepadconnected', (event) => {
      const update = () => {
        zx.kempston[0] = 0;
        for (const gamepad of navigator.getGamepads()) {
          if (!gamepad) continue;
          for (const [index, axis] of gamepad.axes.entries()) {
              if ((index == 0 || index == 4) && axis == -1 ) zx.kempston[0] |= fkempston.kRigth;
              if ((index == 0 || index == 4) && axis == 1 ) zx.kempston[0] |= fkempston.kLeft;
              if ((index == 1 || index == 5 ) && axis == -1 ) zx.kempston[0] |= fkempston.kDown;;
              if ((index == 1 || index == 5 ) && axis == 1 ) zx.kempston[0] |= fkempston.kUp;;
          }
          for (const [index, button] of gamepad.buttons.entries()) {
            if (index == 9) {
                if (!start_button && button.pressed) {
                    start_button = 1; pause();
                } else if (start_button && !button.pressed) start_button = 0;
            } else if (button.pressed) zx.kempston[0] |= fkempston.kFire;;
          }
        }
        requestAnimationFrame(update);
      };
      update();
    });

    pause();

    ctx.fillStyle = "red";
    ctx.font = "16px mono";

    var nosound = 1;
    function pick(event) {
        console.log(actx.state);
        if (actx.state == 'suspended') {
            actx.resume().then(() => {
                console.log(actx.state);
                nosound = 0;
            });
        } else nosound ^= 1;
    }

    function beeper() {
        if (!nosound) {
            var bufferSize = actx.sampleRate/50,
            aBuffer = actx.createBuffer(1, bufferSize, actx.sampleRate),
            output = aBuffer.getChannelData(0);
            var cbuf = zx.swapABuf();
            if (cbuf) for (var i = 0; i < bufferSize; i++) output[i] = (zx.abuf2[i]-128)/63
            else for (var i = 0; i < bufferSize; i++) output[i] = (zx.abuf1[i]-128)/63;
            var aSource = actx.createBufferSource();
            aSource.buffer = aBuffer;
            aSource.loop = false;
            aSource.start(0);
            aSource.connect(actx.destination);
        }
    }

    function go() {
        if (zx.emul_active || !zx.emul_ready) {
            console.log("skip frame");
            return;
        }
        const start = performance.now();
        zx.emul_active = 1;
        zx.emul();
        beeper();
        ctx.putImageData(zx.myImageData, 0, 0);
        if (DEBUG) {
            var hstr = '';
            hstr = "IFF: " + zx.iff1state() + " IM: " + zx.imstate() + " IR: " + toHex(zx.irstate());
            if (zx.haltstate()) hstr += ' HALT';
            ctx.fillText(hstr, 2, 16);
            if(nosound) ctx.fillText("SOUND: OFF", 540, 16);
            const timeTaken = (performance.now() - start);
            html_fps.innerHTML = timeTaken.toFixed(2);
            var regs = " PC: " + toHex(zx.z80[0])
                + " AF: " + toHex(zx.z80[1])
                + " BC: " + toHex(zx.z80[2])
                + " DE: " + toHex(zx.z80[3])
                + " HL: " + toHex(zx.z80[4])
                + " SP: " + toHex(zx.z80[5])
                + " IX: " + toHex(zx.z80[6])
                + " IY: " + toHex(zx.z80[7])
                + " OP: " + toHex(zx.opcode(), 2);
            debug2.innerHTML = regs;
            if (str <= maxStr) {
                debug.innerHTML += "<br>" + " " + regs;
                str++;
            }
        }

        zx.emul_active = 0;
    }

    function pause() {
        if(!pause.active) pause.active = 0;
        if (pause.active ^= 1) pause.intervalID = setInterval(go, interval)
        else {
            clearInterval(pause.intervalID);
            ctx.fillText("PAUSED", 2, height - 16);
        }
    }

    function reset() {
        pause();
        zx.loadROM();
        zx.reset();
        str = 0;
        debug.innerHTML = '';
        pause();
    }
  });

class TZX {
    constructor(ws, cwidth, cheight) {
        this.emul_ready = 0;
        this.cwidth = cwidth;
        this.cheight = cheight;
        this.isize = cwidth*cheight;
        this.buffer = ws.exports.memory.buffer;
        this.vbuf = ws.exports.vbuf.value;
        this.static_data = new Uint32Array(this.buffer, 0, 1024);
        this.ui8ca = new Uint8ClampedArray(this.buffer, this.vbuf, this.isize*4);
        for (var i = 0; i < width*height*4; i++) this.ui8ca[i] = 0x7f;
        this.myImageData = new ImageData(this.ui8ca, cwidth, cheight);
        this.data = this.myImageData.data;
        this.init = ws.exports.init;
        this.emul = ws.exports.emul;
        this.reset = ws.exports.reset;
        this.haltstate = ws.exports.haltstate;
        this.iff1state = ws.exports.iff1state;
        this.iff2state = ws.exports.iff2state;
        this.imstate = ws.exports.imstate;
        this.irstate = ws.exports.irstate;
        this.setKey = ws.exports.setKey;
        this.opcode = ws.exports.opcode;
        this.initSNA48k = ws.exports.initSNA48k;
        this.dumpSNA48k = ws.exports.dumpSNA48k;
        this.swapABuf = ws.exports.swapABuf;
        this.bTime = new Uint8Array(this.buffer, ws.exports.getBTime(), 9);
        this.bDate = new Uint8Array(this.buffer, ws.exports.getBDate(), 12);
        this.zxmem = new Uint8Array(this.buffer, ws.exports.pzxmem(), 65536);
        this.abuf1 = new Uint8Array(this.buffer, ws.exports.babuffer1, 1024);
        this.abuf2 = new Uint8Array(this.buffer, ws.exports.babuffer2, 1024);
        this.z80 = new Uint16Array(this.buffer, ws.exports.z80, 64);
        this.sna = new Uint8Array(this.buffer, ws.exports.SNA, 27);
//        this.ZXKeyboard = new Uint8Array(this.buffer, ws.exports.pZXKeyboard(), 8);;
        this.kempston = new Uint32Array(this.buffer, ws.exports.pkempston(), 1);
        this.itacts = new Uint32Array(this.buffer, ws.exports.itacts, 1);
        this.loadROM();
        this.init();

        this.emul_active = 0;
    }

//    setKey(krow, keys, state) {
//        if (krow >= 8) return;
//        if (state)
//            zx.ZXKeyboard[krow] &= (~keys)
//        else
//            zx.ZXKeyboard[krow] |= keys;
//    }

    loadSNA(sna) {
        this.emul_ready = 0;
        fetch(sna)
          .then((response) => response.arrayBuffer())
          .then((buf) => {
            const rom = new Uint8Array(buf);
            const len = Math.min(buf.byteLength - 27, 48*1024);
            for (var i = 0; i < len; i++) this.zxmem[i + 16384] = rom[i+27];
            for (var i = 0; i <  27; i++) this.sna[i] = rom[i];
            this.initSNA48k();
            this.emul_ready = 1;
          });
    }

    loadROM() {
        this.emul_ready = 0;
        fetch("roms/48.rom")
        .then((resp) => resp.arrayBuffer())
        .then((buf) => {
            const rom = new Uint8Array(buf);
            this.zxmem.set(rom);
            this.emul_ready = 1;
        });
    }
}

function checkKeys(event) {
//        console.log(event);
    var state = 1;
    if (event.type == "keyup") state = 0;
    switch(event.code) {
    case "ShiftLeft": zx.setKey(0, 1, state); break;
    case "KeyZ": zx.setKey(0, 2, state); break;
    case "KeyX": zx.setKey(0, 4, state); break;
    case "KeyC": zx.setKey(0, 8, state); break;
    case "KeyV": zx.setKey(0, 16, state); break;

    case "KeyA": zx.setKey(1, 1, state); break;
    case "KeyS": zx.setKey(1, 2, state); break;
    case "KeyD": zx.setKey(1, 4, state); break;
    case "KeyF": zx.setKey(1, 8, state); break;
    case "KeyG": zx.setKey(1, 16, state); break;

    case "KeyQ": zx.setKey(2, 1, state); break;
    case "KeyW": zx.setKey(2, 2, state); break;
    case "KeyE": zx.setKey(2, 4, state); break;
    case "KeyR": zx.setKey(2, 8, state); break;
    case "KeyT": zx.setKey(2, 16, state); break;

    case "Numpad1":
    case "Digit1": zx.setKey(3, 1, state); break;
    case "Numpad2":
    case "Digit2": zx.setKey(3, 2, state); break;
    case "Numpad3":
    case "Digit3": zx.setKey(3, 4, state); break;
    case "Numpad4":
    case "Digit4": zx.setKey(3, 8, state); break;
    case "Numpad5":
    case "Digit5": zx.setKey(3, 16, state); break;

    case "Numpad0":
    case "Digit0": zx.setKey(4, 1, state); break;
    case "Numpad9":
    case "Digit9": zx.setKey(4, 2, state); break;
    case "Numpad8":
    case "Digit8": zx.setKey(4, 4, state); break;
    case "Numpad7":
    case "Digit7": zx.setKey(4, 8, state); break;
    case "Numpad6":
    case "Digit6": zx.setKey(4, 16, state); break;

    case "KeyP": zx.setKey(5, 1, state); break;
    case "KeyO": zx.setKey(5, 2, state); break;
    case "KeyI": zx.setKey(5, 4, state); break;
    case "KeyU": zx.setKey(5, 8, state); break;
    case "KeyY": zx.setKey(5, 16, state); break;

    case "KeyH": zx.setKey(6, 16, state); break;
    case "KeyJ": zx.setKey(6, 8, state); break;
    case "KeyK": zx.setKey(6, 4, state); break;
    case "KeyL": zx.setKey(6, 2, state); break;
    case "NumpadEnter":
    case "Enter": zx.setKey(6, 1, state); break;

//        case "ControlRight": //SDLK_RSHIFT:
//        case "AltRight": //SDLK_RSHIFT:
    case "Space": zx.setKey(7, 1, state); break;
    case "ShiftRight": zx.setKey(7, 2, state); break;
    case "KeyM": zx.setKey(7, 4, state); break;
    case "KeyN": zx.setKey(7, 8, state); break;
    case "KeyB": zx.setKey(7, 16, state); break;

    case "Backspace":
        zx.setKey(4, 1, state);
        zx.setKey(0, 1, state);
        break;
    case "ArrowRight":
        zx.setKey(4, 4, state);
        zx.setKey(0, 1, state);
        break;
    case "ArrowUp": //up:
        zx.setKey(4, 8, state);
        zx.setKey(0, 1, state);
        break;
    case "ArrowDown": //down:
        zx.setKey(4, 16, state);
        zx.setKey(0, 1, state);
        break;
    case "ArrowLeft": //left:
        zx.setKey(3, 16, state);
        zx.setKey(0, 1, state);
        break;
    case "Semicolon":
        zx.setKey(5, 2, state);
        zx.setKey(7, 2, state);
        break;
    case "Quote":
        zx.setKey(5, 1, state);
        zx.setKey(7, 2, state);
        break;
    case "Equal":
        zx.setKey(6, 2, state);
        zx.setKey(7, 2, state);
        break;
    case "NumpadSubtract":
    case "Minus":
        zx.setKey(6, 8, state);
        zx.setKey(7, 2, state);
        break;
    case "NumpadAdd":
        zx.setKey(6, 4, state);
        zx.setKey(7, 2, state);
        break;
    case "NumpadMultiply":
        zx.setKey(7, 2 | 16, state);
        break;
    case "NumpadDivide":
    case "Slash":
        zx.setKey(0, 16, state);
        zx.setKey(7, 2, state);
        break;
    case "Comma":
        zx.setKey(7, 2 | 8, state);
        break;
    case "NumpadDecimal":
    case "Period":
        zx.setKey(7, 2 | 4, state);
        break;
    default:
        return true;
    };
    event.preventDefault();
    return false;
}

});
