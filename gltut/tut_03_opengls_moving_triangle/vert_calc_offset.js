var VERTEX_SHADER_SRC = `#version 100
    attribute vec4 a_position;
    uniform float u_loopDuration;
    uniform float u_time;

    void
    main()
    {
        float timeScale = 3.14159 * 2.0 / u_loopDuration;
        float currentTime = mod(u_time, u_loopDuration);
        vec4 totalOffset = vec4(cos(currentTime * timeScale) * 0.5,
                                sin(currentTime * timeScale) * 0.5,
                                0.0,
                                0.0);
        gl_Position = a_position + totalOffset;
    }
`
var FRAGMENT_SHADER_SRC = `#version 100
    precision mediump float;

    void
    main()
    {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
`
var VERTEX_ATTR_INDEX_POSITION = 0;
var SHADER_ATTRIB_LOCATIONS = [
    {name: "a_position", location: VERTEX_ATTR_INDEX_POSITION},
];
var VERTEX_POSITIONS = new Float32Array([
     0.25,  0.25, 0.0, 1.0,
     0.25, -0.25, 0.0, 1.0,
    -0.25, -0.25, 0.0, 1.0,
]);
var LOOP_DURATION = 5.0;

var gl;
var theProgram;
var triangleVBO;
var timeUniformLoc;

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
        let loopDurationUniformLoc;

        loopDurationUniformLoc = gl.getUniformLocation(theProgram, "u_loopDuration");
        timeUniformLoc = gl.getUniformLocation(theProgram, "u_time");

        gl.useProgram(theProgram);
        gl.uniform1f(loopDurationUniformLoc, LOOP_DURATION);
        gl.useProgram(null);
    }

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

    var draw = function(timeInMilliseconds) {
        var timeInSeconds = timeInMilliseconds / 1000.0;

        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);

        gl.useProgram(theProgram);


        gl.bindBuffer(gl.ARRAY_BUFFER, triangleVBO);
        gl.enableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
        gl.vertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 4, gl.FLOAT, false, 0, 0);
        gl.uniform1f(timeUniformLoc, timeInSeconds);
        gl.drawArrays(gl.TRIANGLES, 0, 3);
        gl.uniform1f(timeUniformLoc, timeInSeconds + LOOP_DURATION / 2.0);
        gl.drawArrays(gl.TRIANGLES, 0, 3);
        gl.disableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);

        gl.useProgram(null);

        window.requestAnimationFrame(draw);
    };
    window.requestAnimationFrame(draw);
}
