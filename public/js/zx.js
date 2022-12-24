const interval = 25;
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

const ctx = canvas.getContext('2d');

if (ctx) {
    ctx.canvas.width  = width;
    ctx.canvas.height  = height;
    fetch("js/zxemul.wasm")
      .then((response) => response.arrayBuffer())
      .then((bytes) => WebAssembly.instantiate(bytes))
      .then((results) => {
        console.log(results.instance);
        const zx = new TZX(results.instance, width, height);
        zx.init();

        pbutton.onclick = pause;
        cbutton.onclick = reset;
//        lbutton.onclick = function(){pause(); zx.loadSNA("roms/dizzy_2.sna"); pause() };
        rlbutton.onclick = function(){str = 0; debug.innerHTML = '';};
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

        const fkempston = {kLeft:1,kRigth:2,kUp:4,kDown:8,kFire:16};

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

        function toHex(d, n = 4) {
            return (d+0x10000).toString(16).substr(-n).toUpperCase();
        }

        const maxStr = 64;
        var str = 0;
        function go() {
            const start = performance.now();
            zx.emul();
            ctx.putImageData(zx.myImageData, 0, 0);
            const timeTaken = (performance.now() - start);
            html_fps.innerHTML = timeTaken;

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
            if ((opCounter >= 100000) && (str <= maxStr)) {
                debug.innerHTML += "<br>" + opCounter + regs;
                str++;
            }
        }
        function pause() {
            if(!pause.active) pause.active = 0;
            if (pause.active ^= 1) pause.intervalID = setInterval(go, interval)
            else clearInterval(pause.intervalID);
        }

        function checkKeys(event) {
            console.log(event);
            var state = 1;
            if (event.type == "keyup") state = 0;
            switch(event.keyCode) {
            case 16:
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x1);
                    else
                        zx.ZXKeyboard[0] |= (0x1);
                break;
            case 90:
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x2);
                    else
                        zx.ZXKeyboard[0] |= (0x2);
                break;
            case 88:
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x4);
                    else
                        zx.ZXKeyboard[0] |= (0x4);
                break;
            case 67:
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x8);
                    else
                        zx.ZXKeyboard[0] |= (0x8);
                break;
            case 86:
                    if (state)
                        zx.ZXKeyboard[0] &= (~0x10);
                    else
                        zx.ZXKeyboard[0] |= (0x10);
                break;

            case 65:
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x1);
                    else
                        zx.ZXKeyboard[1] |= (0x1);
                break;
            case 83:
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x2);
                    else
                        zx.ZXKeyboard[1] |= (0x2);
                break;
            case 68:
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x4);
                    else
                        zx.ZXKeyboard[1] |= (0x4);
                break;
            case 70:
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x8);
                    else
                        zx.ZXKeyboard[1] |= (0x8);
                break;
            case 71:
                    if (state)
                        zx.ZXKeyboard[1] &= (~0x10);
                    else
                        zx.ZXKeyboard[1] |= (0x10);
                break;

            case 81:
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x1);
                    else
                        zx.ZXKeyboard[2] |= (0x1);
                break;
            case 87:
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x2);
                    else
                        zx.ZXKeyboard[2] |= (0x2);
                break;
            case 69:
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x4);
                    else
                        zx.ZXKeyboard[2] |= (0x4);
                break;
            case 82:
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x8);
                    else
                        zx.ZXKeyboard[2] |= (0x8);
                break;
            case 84:
                    if (state)
                        zx.ZXKeyboard[2] &= (~0x10);
                    else
                        zx.ZXKeyboard[2] |= (0x10);
                break;

            case 49:
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x1);
                    else
                        zx.ZXKeyboard[3] |= (0x1);
                break;
            case 50:
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x2);
                    else
                        zx.ZXKeyboard[3] |= (0x2);
                break;
            case 51:
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x4);
                    else
                        zx.ZXKeyboard[3] |= (0x4);
                break;
            case 52:
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x8);
                    else
                        zx.ZXKeyboard[3] |= (0x8);
                break;
            case 53: //SDLK_5:
                    if (state)
                        zx.ZXKeyboard[3] &= (~0x10);
                    else
                        zx.ZXKeyboard[3] |= (0x10);
                break;

            case 48: //SDLK_0:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x1);
                    else
                        zx.ZXKeyboard[4] |= (0x1);
                break;
            case 57: //SDLK_9:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x2);
                    else
                        zx.ZXKeyboard[4] |= (0x2);
                break;
            case 56: //SDLK_8:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x4);
                    else
                        zx.ZXKeyboard[4] |= (0x4);
                break;
            case 55: //SDLK_7:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x8);
                    else
                        zx.ZXKeyboard[4] |= (0x8);
                break;
            case 54: //SDLK_6:
                    if (state)
                        zx.ZXKeyboard[4] &= (~0x10);
                    else
                        zx.ZXKeyboard[4] |= (0x10);
                break;

            case 80: //SDLK_p:
                    if (state)
                        zx.ZXKeyboard[5]  &= (~0x1);
                    else
                        zx.ZXKeyboard[5] |= (0x1);
                break;
            case 79: //SDLK_o:
                    if (state)
                        zx.ZXKeyboard[5] &= (~0x2);
                    else
                        zx.ZXKeyboard[5] |= (0x2);
                break;
            case 73: //SDLK_i:
                    if (state)
                        zx.ZXKeyboard[5] &= (~0x4);
                    else
                        zx.ZXKeyboard[5] |= (0x4);
                break;
            case 85: //SDLK_u:
                    if (state)
                        zx.ZXKeyboard[5] &= (~0x8);
                    else
                        zx.ZXKeyboard[5] |= (0x8);
                break;
            case 89: //SDLK_y:
                    if (state)
                        zx.ZXKeyboard[5] &= (~0x10);
                    else
                        zx.ZXKeyboard[5] |= (0x10);
                break;

            case 72:
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x10);
                    else
                        zx.ZXKeyboard[6] |= (0x10);
                break;
            case 74:
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x8);
                    else
                        zx.ZXKeyboard[6] |= (0x8);
                break;
            case 75:
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x4);
                    else
                        zx.ZXKeyboard[6] |= (0x4);
                break;
            case 76:
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x2);
                    else
                        zx.ZXKeyboard[6] |= (0x2);
                break;
            case 13:
                    if (state)
                        zx.ZXKeyboard[6] &= (~0x1);
                    else
                        zx.ZXKeyboard[6] |= (0x1);
                break;

            case 32: //SDLK_SPACE:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x1);
                    else
                        zx.ZXKeyboard[7] |= (0x1);
                break;
            case 17: //SDLK_RSHIFT:
            case 18: //SDLK_RSHIFT:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x2);
                    else
                        zx.ZXKeyboard[7] |= (0x2);
                break;
            case 77: //SDLK_m:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x4);
                    else
                        zx.ZXKeyboard[7] |= (0x4);
                break;
            case 78: //SDLK_n:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x8);
                    else
                        zx.ZXKeyboard[7] |= (0x8);
                break;
            case 66: //SDLK_b:
                    if (state)
                        zx.ZXKeyboard[7] &= (~0x10);
                    else
                        zx.ZXKeyboard[7] |= (0x10);
                break;

            case 8: //backspace:
                    if (state) {
                        zx.ZXKeyboard[4] &= (~0x1);
                        zx.ZXKeyboard[0] &= (~0x1);

                    } else {
                        zx.ZXKeyboard[0] |= (0x1);
                        zx.ZXKeyboard[4] |= (0x1);
                    }
                break;
            case 39: //right:
                    if (state) {
                        zx.ZXKeyboard[4] &= (~0x4);
                        zx.ZXKeyboard[0] &= (~0x1);
                    } else {
                        zx.ZXKeyboard[4] |= (0x4);
                        zx.ZXKeyboard[0] |= (0x1);
                    }
                break;
            case 38: //up:
                    if (state) {
                        zx.ZXKeyboard[4] &= (~0x8);
                        zx.ZXKeyboard[0] &= (~0x1);
                    } else {
                        zx.ZXKeyboard[4] |= (0x8);
                        zx.ZXKeyboard[0] |= (0x1);
                    }
                break;
            case 40: //down:
                    if (state) {
                        zx.ZXKeyboard[4] &= (~0x10);
                        zx.ZXKeyboard[0] &= (~0x1);
                    } else {
                        zx.ZXKeyboard[4] |= (0x10);
                        zx.ZXKeyboard[0] |= (0x1);
                    }
                break;
            case 37: //left:
                    if (state) {
                        zx.ZXKeyboard[3] &= (~0x10);
                        zx.ZXKeyboard[0] &= (~0x1);
                    } else {
                        zx.ZXKeyboard[3] |= (0x10);
                        zx.ZXKeyboard[0] |= (0x1);
                    }
                break;
            };
        };

        function pick(event) {
            const bounding = canvas.getBoundingClientRect();
            const x = (event.clientX - bounding.left) * cwidth/event.target.clientWidth;
            const y = (event.clientY - bounding.top) * cheight/event.target.clientHeight;
            if ((x < 2) || (y < 2) || (y > (cheight - 2)) || (x > (cwidth - 2))) return;
            const xy = figure.add_node(x, y);
            go();
            status.innerHTML = "pick("+xy[0]+", "+xy[1]+"); nodes: " + figure.nodes_count();
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
        for (var i = 0; i < width*height*4; i++) {
            this.ui8ca[i] = 0x7f;
        }
        this.myImageData = new ImageData(this.ui8ca, cwidth, cheight);
        this.data = this.myImageData.data;
        this.init = ws.exports.init;
        this.emul = ws.exports.emul;
        this.opCounter = ws.exports.opCounter;
        this.opcode = ws.exports.opcode;
        this.loadSNA48k = ws.exports.loadSNA48k;
        this.zxmem = new Uint8Array(this.buffer, ws.exports.zxmem, 65536);
        this.z80 = new Uint16Array(this.buffer, ws.exports.z80, 64);
        this.sna = new Uint8Array(this.buffer, ws.exports.SNA, 27);
        this.ZXKeyboard = new Uint8Array(this.buffer, ws.exports.ZXKeyboard, 8);;
        this.kempston = new Uint32Array(this.buffer, ws.exports.kempston, 1);;
        this.loadROM();
        this.init();
    }

    async loadSNA(sna) {
        var resp = await fetch(sna);
        var blob = await resp.blob();
        var buf = await blob.arrayBuffer();
        var sna = new Uint8Array(buf);
        for (var i = 0; i< buf.byteLength; i++)
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

