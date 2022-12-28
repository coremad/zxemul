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
const mbut = document.getElementById("dropbtn")


const ctx = canvas.getContext('2d');

mbut.onclick = function() {
    document.getElementById("myDropdown").classList.toggle("show");
}

if (ctx) {
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
    fetch("js/zxemul.wasm")
      .then((response) => response.arrayBuffer())
      .then((bytes) => WebAssembly.instantiate(bytes))
      .then((results) => {
        console.log(results.instance);
        zx = new TZX(results.instance, width, height);
        zx.init();

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
                zx.loadSNA48k();
                ifile.value = '';
                str = 0; debug.innerHTML = "";
                pause();
           }
        }
        lbutton.onclick = function() { ifile.click(); };

        sbutton.onclick = function() {
            pause();
            zx.saveSNA48k();
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
//        canvas.addEventListener('click', event => pick(event));
        window.addEventListener('keydown', event => checkKeys(event), false);
        window.addEventListener('keyup', event => checkKeys(event), false);

        let elements = document.getElementsByClassName("fetch_sna");
        for(let i = 0; i < elements.length; i++) elements[i].onclick = function (e) {
                pause();
                zx.loadSNA('roms/' + e.srcElement.innerHTML + '.sna');
                str = 0; debug.innerHTML = "";
                pause();
            }

        elements = document.getElementsByClassName("model");
        for(let i = 0; i < elements.length; i++) elements[i].onclick = function (e) {
                zx.itacts[0] = e.target.getAttribute("data");
                islider.value = zx.itacts[0];
                itacts.innerHTML = ("000000" + zx.itacts[0]).slice(-6);
            }

        const fkempston = {kLeft:1, kRigth:2, kUp:4, kDown:8, kFire:16};

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
                if (button.value != 0) zx.kempston[0] |= fkempston.kFire;;
              }
            }
            requestAnimationFrame(update);
          };
          update();
        });
        pause();

        ctx.fillStyle = "red";
        ctx.font = "16px mono";

        function go() {
            const start = performance.now();
            zx.emul_active = 1;
            zx.emul();
            zx.emul_active = 0;
            ctx.putImageData(zx.myImageData, 0, 0);
            if (DEBUG) {
                var hstr = '';
                hstr = "IFF: " + zx.iff1state() + " IM: " + zx.imstate() + " IR: " + toHex(zx.irstate());
                if (zx.haltstate()) hstr += ' HALT';
                ctx.fillText(hstr, 2, 16);
                const timeTaken = (performance.now() - start);
                html_fps.innerHTML = timeTaken.toFixed(2);
                var opCounter = zx.opCounter();
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
                if ((opCounter >= 0) && (str <= maxStr)) {
                    debug.innerHTML += "<br>" + " " + opCounter + regs;
                    str++;
//                    showregs = 0;
                }
            }
        }
        function pause() {
            if(!pause.active) pause.active = 0;
            if (pause.active ^= 1) pause.intervalID = setInterval(go, interval)
            else {
                clearInterval(pause.intervalID);
                ctx.fillText("PAUSED", 2, height - 16);
            }
        }

        function checkKeys(event) {
//            console.log(event);
            var state = 1;
            if (event.type == "keyup") state = 0;
            switch(event.code) {
            case "ShiftLeft":
//            case "ControlLeft":
//            case "AltLeft":
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x1);
                    else
                        zx.ZXKeyboard[0] |= (0x1);
                break;
            case "KeyZ":
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x2);
                    else
                        zx.ZXKeyboard[0] |= (0x2);
                break;
            case "KeyX":
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x4);
                    else
                        zx.ZXKeyboard[0] |= (0x4);
                break;
            case "KeyC":
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x8);
                    else
                        zx.ZXKeyboard[0] |= (0x8);
                break;
            case "KeyV":
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x10);
                    else
                        zx.ZXKeyboard[0] |= (0x10);
                break;

            case "KeyA":
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x1);
                    else
                        zx.ZXKeyboard[1] |= (0x1);
                break;
            case "KeyS":
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x2);
                    else
                        zx.ZXKeyboard[1] |= (0x2);
                break;
            case "KeyD":
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x4);
                    else
                        zx.ZXKeyboard[1] |= (0x4);
                break;
            case "KeyF":
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x8);
                    else
                        zx.ZXKeyboard[1] |= (0x8);
                break;
            case "KeyG":
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x10);
                    else
                        zx.ZXKeyboard[1] |= (0x10);
                break;

            case "KeyQ":
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x1);
                    else
                        zx.ZXKeyboard[2] |= (0x1);
                break;
            case "KeyW":
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x2);
                    else
                        zx.ZXKeyboard[2] |= (0x2);
                break;
            case "KeyE":
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x4);
                    else
                        zx.ZXKeyboard[2] |= (0x4);
                break;
            case "KeyR":
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x8);
                    else
                        zx.ZXKeyboard[2] |= (0x8);
                break;
            case "KeyT":
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x10);
                    else
                        zx.ZXKeyboard[2] |= (0x10);
                break;

            case "Numpad1":
            case "Digit1":
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x1);
                    else
                        zx.ZXKeyboard[3] |= (0x1);
                break;
            case "Numpad2":
            case "Digit2":
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x2);
                    else
                        zx.ZXKeyboard[3] |= (0x2);
                break;
            case "Numpad3":
            case "Digit3":
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x4);
                    else
                        zx.ZXKeyboard[3] |= (0x4);
                break;
            case "Numpad4":
            case "Digit4":
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x8);
                    else
                        zx.ZXKeyboard[3] |= (0x8);
                break;
            case "Numpad5":
            case "Digit5": //SDLK_5:
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x10);
                    else
                        zx.ZXKeyboard[3] |= (0x10);
                break;

            case "Numpad0":
            case "Digit0": //SDLK_0:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x1);
                    else
                        zx.ZXKeyboard[4] |= (0x1);
                break;
            case "Numpad9":
            case "Digit9": //SDLK_9:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x2);
                    else
                        zx.ZXKeyboard[4] |= (0x2);
                break;
            case "Numpad8":
            case "Digit8": //SDLK_8:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x4);
                    else
                        zx.ZXKeyboard[4] |= (0x4);
                break;
            case "Numpad7":
            case "Digit7": //SDLK_7:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x8);
                    else
                        zx.ZXKeyboard[4] |= (0x8);
                break;
            case "Numpad6":
            case "Digit6": //SDLK_6:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x10);
                    else
                        zx.ZXKeyboard[4] |= (0x10);
                break;

            case "KeyP": //SDLK_p:
                    if (state)
                        zx.ZXKeyboard[5]  &= (~0x1);
                    else
                        zx.ZXKeyboard[5] |= (0x1);
                break;
            case "KeyO": //SDLK_o:
                    if (state)
                        zx.ZXKeyboard[5] &= (~0x2);
                    else
                        zx.ZXKeyboard[5] |= (0x2);
                break;
            case "KeyI": //SDLK_i:
                    if (state)
                        zx.ZXKeyboard[5] &= (~0x4);
                    else
                        zx.ZXKeyboard[5] |= (0x4);
                break;
            case "KeyU": //SDLK_u:
                    if (state)
                        zx.ZXKeyboard[5] &= (~0x8);
                    else
                        zx.ZXKeyboard[5] |= (0x8);
                break;
            case "KeyY": //SDLK_y:
                    if (state)
                        zx.ZXKeyboard[5] &= (~0x10);
                    else
                        zx.ZXKeyboard[5] |= (0x10);
                break;

            case "KeyH":
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x10);
                    else
                        zx.ZXKeyboard[6] |= (0x10);
                break;
            case "KeyJ":
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x8);
                    else
                        zx.ZXKeyboard[6] |= (0x8);
                break;
            case "KeyK":
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x4);
                    else
                        zx.ZXKeyboard[6] |= (0x4);
                break;
            case "KeyL":
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x2);
                    else
                        zx.ZXKeyboard[6] |= (0x2);
                break;
            case "NumpadEnter":
            case "Enter":
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x1);
                    else
                        zx.ZXKeyboard[6] |= (0x1);
                break;

            case "Space": //SDLK_SPACE:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x1);
                    else
                        zx.ZXKeyboard[7] |= (0x1);
                break;
            case "ShiftRight": //SDLK_RSHIFT:
//            case "ControlRight": //SDLK_RSHIFT:
//            case "AltRight": //SDLK_RSHIFT:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x2);
                    else
                        zx.ZXKeyboard[7] |= (0x2);
                break;
            case "KeyM": //SDLK_m:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x4);
                    else
                        zx.ZXKeyboard[7] |= (0x4);
                break;
            case "KeyN": //SDLK_n:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x8);
                    else
                        zx.ZXKeyboard[7] |= (0x8);
                break;
            case "KeyB": //SDLK_b:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x10);
                    else
                        zx.ZXKeyboard[7] |= (0x10);
                break;

            case "Backspace": //backspace:
                    if (state) {
                        zx.ZXKeyboard[4] &= (~0x1);
                        zx.ZXKeyboard[0] &= (~0x1);

                    } else {
                        zx.ZXKeyboard[0] |= (0x1);
                        zx.ZXKeyboard[4] |= (0x1);
                    }
                break;
            case "ArrowRight": //right:
                    if (state) {
                        zx.ZXKeyboard[4] &= (~0x4);
                        zx.ZXKeyboard[0] &= (~0x1);
                    } else {
                        zx.ZXKeyboard[4] |= (0x4);
                        zx.ZXKeyboard[0] |= (0x1);
                    }
                break;
            case "ArrowUp": //up:
                    if (state) {
                        zx.ZXKeyboard[4] &= (~0x8);
                        zx.ZXKeyboard[0] &= (~0x1);
                    } else {
                        zx.ZXKeyboard[4] |= (0x8);
                        zx.ZXKeyboard[0] |= (0x1);
                    }
                break;
            case "ArrowDown": //down:
                    if (state) {
                        zx.ZXKeyboard[4] &= (~0x10);
                        zx.ZXKeyboard[0] &= (~0x1);
                    } else {
                        zx.ZXKeyboard[4] |= (0x10);
                        zx.ZXKeyboard[0] |= (0x1);
                    }
                break;
            case "ArrowLeft": //left:
                    if (state) {
                        zx.ZXKeyboard[3] &= (~0x10);
                        zx.ZXKeyboard[0] &= (~0x1);
                    } else {
                        zx.ZXKeyboard[3] |= (0x10);
                        zx.ZXKeyboard[0] |= (0x1);
                    }
                break;
            case "Semicolon":
                    if (state) {
                        zx.ZXKeyboard[5] &= (~0x2);
                        zx.ZXKeyboard[7] &= (~0x2);
                    } else {
                        zx.ZXKeyboard[5] |= (0x2);
                        zx.ZXKeyboard[7] |= (0x2);
                    }
                break;
            case "Quote":
                    if (state) {
                        zx.ZXKeyboard[5] &= (~0x1);
                        zx.ZXKeyboard[7] &= (~0x2);
                    } else {
                        zx.ZXKeyboard[5] |= (0x1);
                        zx.ZXKeyboard[7] |= (0x2);
                    }
                break;
            case "Equal":
                    if (state) {
                        zx.ZXKeyboard[6] &= (~0x2);
                        zx.ZXKeyboard[7] &= (~0x2);
                    } else {
                        zx.ZXKeyboard[6] |= (0x2);
                        zx.ZXKeyboard[7] |= (0x2);
                    }
                break;
            case "NumpadSubtract":
            case "Minus":
                    if (state) {
                        zx.ZXKeyboard[6] &= (~0x8);
                        zx.ZXKeyboard[7] &= (~0x2);
                    } else {
                        zx.ZXKeyboard[6] |= (0x8);
                        zx.ZXKeyboard[7] |= (0x2);
                    }
                break;
            case "NumpadAdd":
                    if (state) {
                        zx.ZXKeyboard[6] &= (~0x4);
                        zx.ZXKeyboard[7] &= (~0x2);
                    } else {
                        zx.ZXKeyboard[6] |= (0x4);
                        zx.ZXKeyboard[7] |= (0x2);
                    }
                break;
            case "NumpadMultiply":
                    if (state) {
                        zx.ZXKeyboard[7] &= (~0x10);
                        zx.ZXKeyboard[7] &= (~0x2);
                    } else {
                        zx.ZXKeyboard[7] |= (0x10);
                        zx.ZXKeyboard[7] |= (0x2);
                    }
                break;
            case "NumpadDivide":
            case "Slash":
                    if (state) {
                        zx.ZXKeyboard[0] &= (~0x10);
                        zx.ZXKeyboard[7] &= (~0x2);
                    } else {
                        zx.ZXKeyboard[0] |= (0x10);
                        zx.ZXKeyboard[7] |= (0x2);
                    }
                break;
            case "Comma":
                    if (state) {
                        zx.ZXKeyboard[7] &= (~0x8);
                        zx.ZXKeyboard[7] &= (~0x2);
                    } else {
                        zx.ZXKeyboard[7] |= (0x8);
                        zx.ZXKeyboard[7] |= (0x2);
                    }
                break;
            case "NumpadDecimal":
            case "Period":
                    if (state) {
                        zx.ZXKeyboard[7] &= (~0x4);
                        zx.ZXKeyboard[7] &= (~0x2);
                    } else {
                        zx.ZXKeyboard[7] |= (0x4);
                        zx.ZXKeyboard[7] |= (0x2);
                    }
                break;
            };
            event.preventDefault();
            return false;
        }

        function pick(event) {
        }

        function reset() {
            pause();
            zx.loadROM();
            zx.init();
            str = 0;
            debug.innerHTML = '';
            pause();
        }
      });
} else status.innerHTML = 'ERROR canvas.getContext';

class TZX {
    constructor(ws, cwidth, cheight) {
        const align = 1024;
        var static_space = (~~(ws.exports.__data_end/align))*align;
        if (ws.exports.__data_end%align) static_space += align;
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
        this.opCounter = ws.exports.opCounter;
        this.haltstate = ws.exports.haltstate;
        this.iff1state = ws.exports.iff1state;
        this.iff2state = ws.exports.iff2state;
        this.imstate = ws.exports.imstate;
        this.irstate = ws.exports.irstate;
        this.opcode = ws.exports.opcode;
        this.loadSNA48k = ws.exports.loadSNA48k;
        this.saveSNA48k = ws.exports.saveSNA48k;
        this.zxmem = new Uint8Array(this.buffer, ws.exports.zxmem, 65536);
        this.z80 = new Uint16Array(this.buffer, ws.exports.z80, 64);
        this.sna = new Uint8Array(this.buffer, ws.exports.SNA, 27);
        this.ZXKeyboard = new Uint8Array(this.buffer, ws.exports.ZXKeyboard, 8);;
        this.kempston = new Uint32Array(this.buffer, ws.exports.kempston, 1);
        this.itacts = new Uint32Array(this.buffer, ws.exports.itacts, 1);
        this.loadROM();
        this.init();
        this.emul_active = 0;
    }

    async loadSNA(sna) {
        var resp = await fetch(sna);
        var blob = await resp.blob();
        var buf = await blob.arrayBuffer();
        var sna = new Uint8Array(buf);
        var len = Math.max(buf.byteLength - 27, 48*1024);
        for (var i = 0; i < len; i++)
            this.zxmem[i + 16384] = sna[i+27];
        for (var i = 0; i <  27; i++) this.sna[i] = sna[i];
        this.loadSNA48k();
    }
    async loadROM() {
        var resp = await fetch("roms/48.rom");
        var blob = await resp.blob();
        var buf = await blob.arrayBuffer();
        var sna = await new Uint8Array(buf);
        for (var i = 0; i< 16384; i++) this.zxmem[i] = sna[i];
    }
}

});