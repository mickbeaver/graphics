const VERTEX_SHADER_SRC = `#version 100
    attribute vec4 a_position;
    attribute vec4 a_color;
    varying vec4 v_color;

    void
    main()
    {
        v_color = a_color;
        gl_Position = a_position;
    }
`
const FRAGMENT_SHADER_SRC = `#version 100
    precision mediump float;
    varying vec4 v_color;

    void
    main()
    {
        gl_FragColor = v_color;
    }
`
const VERTEX_ATTR_INDEX_POSITION = 0;
const VERTEX_ATTR_INDEX_COLOR = 1;
const SHADER_ATTRIB_LOCATIONS = [
    {name: "a_position", location: VERTEX_ATTR_INDEX_POSITION},
    {name: "a_color",    location: VERTEX_ATTR_INDEX_COLOR},
];
const TRIANGLE_MESH = new Float32Array([
     0.0,  0.5, 0.0,
     1.0,  0.0, 0.0, // red
    -0.5, -0.5, 0.0,
     0.0,  1.0, 0.0, // green
     0.5, -0.5, 0.0,
     0.0,  0.0, 1.0, // blue
]);
const TRIANGLE_INDICES = new Int16Array([0, 1, 2]);
const SIZE_OF_FLOAT = 4;
const NUM_COMPONENTS_POSITION = 3;
const NUM_COMPONENTS_COLOR = 3;
const VERTEX_STRIDE_BYTES = SIZE_OF_FLOAT * (NUM_COMPONENTS_POSITION + NUM_COMPONENTS_COLOR);

var gl;
var theProgram;
var triangleVertexBuffer;
var triangleIndexBuffer;
var rotationUniformLoc;

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
    if (theProgram) {
        rotationUniformLoc = gl.getUniformLocation(theProgram, "u_rotation");
    }
    gl.deleteShader(vertexShader);
    gl.deleteShader(fragmentShader);
}

function initializeTriangleBuffers() {
    triangleVertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, triangleVertexBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, TRIANGLE_MESH, gl.STATIC_DRAW);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    triangleIndexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, TRIANGLE_INDICES, gl.STATIC_DRAW);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, null);
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
    initializeTriangleBuffers();

    var draw = function(timeInMilliseconds) {
        
        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);

        gl.useProgram(theProgram);

        gl.bindBuffer(gl.ARRAY_BUFFER, triangleVertexBuffer);
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
        gl.enableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
        gl.enableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);
        gl.vertexAttribPointer(VERTEX_ATTR_INDEX_POSITION,
                               NUM_COMPONENTS_POSITION,
                               gl.FLOAT,
                               false,
                               VERTEX_STRIDE_BYTES,
                               0);
        gl.vertexAttribPointer(VERTEX_ATTR_INDEX_COLOR,
                               NUM_COMPONENTS_COLOR,
                               gl.FLOAT,
                               false,
                               VERTEX_STRIDE_BYTES,
                               SIZE_OF_FLOAT * NUM_COMPONENTS_POSITION);
        gl.drawElements(gl.TRIANGLES, TRIANGLE_INDICES.length, gl.UNSIGNED_SHORT, 0);
        gl.disableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
        gl.disableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);

        gl.useProgram(null);

        window.requestAnimationFrame(draw);
    };
    window.requestAnimationFrame(draw);
}
