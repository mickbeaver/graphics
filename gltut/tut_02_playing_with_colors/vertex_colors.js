var gl;
var VERTEX_SHADER_SRC = `#version 100
    attribute vec4 a_position;
    attribute vec4 a_color;
    varying vec4 v_color;

    void
    main()
    {
        gl_Position = a_position;
        v_color = a_color;
    }
`
var FRAGMENT_SHADER_SRC = `#version 100
    precision mediump float;

    varying vec4 v_color;

    void
    main()
    {
        gl_FragColor = v_color;
    }
`
var VERTEX_ATTR_INDEX_POSITION = 0;
var VERTEX_ATTR_INDEX_COLOR    = 1;
var SHADER_ATTRIB_LOCATIONS = [
    {name: "a_position", location: VERTEX_ATTR_INDEX_POSITION},
    {name: "a_color", location: VERTEX_ATTR_INDEX_COLOR},
];
var VERTEX_POSITIONS = new Float32Array([
     0.0,   0.5,   0.0, 1.0,
     0.5,  -0.366, 0.0, 1.0,
    -0.5,  -0.366, 0.0, 1.0,
     1.0,   0.0,   0.0, 1.0,
     0.0,   1.0,   0.0, 1.0,
     0.0,   0.0,   1.0, 1.0,
]);

var theProgram;
var triangleVBO;
var windowHeightUniformLoc;

function createShader(shaderType, shaderSrc) {
    var shader;
    var status;

    shader = gl.createShader(shaderType);
    gl.shaderSource(shader, shaderSrc);
    gl.compileShader(shader);
    status = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
    if (!status) {
        let errorMessage = gl.getShaderInfoLog(shader);
        console.log('Failed to compile shader:  ' + errorMessage);
        gl.deleteShader(shader);
        shader = null;
    }

    return shader;
}

function createProgram(vertexShader, fragmentShader) {
    var program;
    var status;

    program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    for (let attribLocation of SHADER_ATTRIB_LOCATIONS) {
        gl.bindAttribLocation(program, attribLocation.location, attribLocation.name);
    }
    gl.linkProgram(program);

    status = gl.getProgramParameter(program, gl.LINK_STATUS);
    if (!status) {
        let errorMessage = gl.getProgramInfoLog(program);
        console.log('Failed to link shader:  ' + errorMessage);
        gl.deleteProgram(program);
        program = null;
    }

    gl.detachShader(program, vertexShader);
    gl.detachShader(program, fragmentShader);

    return program;
}

function initializeProgram(canvas) {
    var vertexShader;
    var fragmentShader;

    vertexShader = createShader(gl.VERTEX_SHADER, VERTEX_SHADER_SRC);
    fragmentShader = createShader(gl.FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);
    theProgram = createProgram(vertexShader, fragmentShader);

    gl.deleteShader(vertexShader);
    gl.deleteShader(fragmentShader);
}

function initializeVertexBuffer() {
    triangleVBO = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, triangleVBO);
    gl.bufferData(gl.ARRAY_BUFFER, VERTEX_POSITIONS, gl.STATIC_DRAW);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
}

function initWebGL(canvas) {
    var context = canvas.getContext("webgl");
    if (!context) {
        alert("Unable to initialize WebGL. Your browser may not support it.");
    }

    return context;
}

function start() {
    var canvas = document.getElementById("glcanvas");
    gl = initWebGL(canvas);
    if (!gl) {
        return;
    }

    initializeProgram(canvas);
    initializeVertexBuffer();

    var draw = function() {
        window.requestAnimationFrame(draw);

        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);

        gl.useProgram(theProgram);

        gl.bindBuffer(gl.ARRAY_BUFFER, triangleVBO);

        gl.enableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
        gl.enableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);

        gl.vertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 4, gl.FLOAT, false, 0, 0);
        gl.vertexAttribPointer(VERTEX_ATTR_INDEX_COLOR, 4, gl.FLOAT, false, 0, 48);

        gl.drawArrays(gl.TRIANGLES, 0, 3);

        gl.disableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
        gl.disableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);

        gl.useProgram(null);
    };
    draw();
}
