const VERTEX_SHADER_SRC = `#version 100
    uniform mat4 u_rotation;
    attribute vec4 a_position;
    attribute vec4 a_color;
    varying vec4 v_color;

    void
    main()
    {
        v_color = a_color;
        gl_Position = u_rotation * a_position;
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
const CUBE_INDICES = new Int16Array([0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23]);
const CUBE_MESH = new Float32Array([
     0.5,  0.5,  0.5, // face 0, vert 0
     1.0,  0.0,  0.0, // red
     0.5, -0.5,  0.5, // face 0, vert 1
     1.0,  0.0,  0.0, // red
     0.5, -0.5, -0.5, // face 0, vert 2
     1.0,  0.0,  0.0, // red
     0.5,  0.5, -0.5, // face 0, vert 3
     1.0,  0.0,  0.0, // red
     0.5,  0.5, -0.5, // face 1, vert 4
     0.0,  1.0,  0.0, // green
     0.5, -0.5, -0.5, // face 1, vert 5
     0.0,  1.0,  0.0, // green
    -0.5, -0.5, -0.5, // face 1, vert 6
     0.0,  1.0,  0.0, // green
    -0.5,  0.5, -0.5, // face 1, vert 7
     0.0,  1.0,  0.0, // green
    -0.5,  0.5, -0.5, // face 2, vert 8
     0.0,  0.0,  1.0, // blue
    -0.5, -0.5, -0.5, // face 2, vert 9
     0.0,  0.0,  1.0, // blue
    -0.5, -0.5,  0.5, // face 2, vert 10
     0.0,  0.0,  1.0, // blue
    -0.5,  0.5,  0.5, // face 2, vert 11
     0.0,  0.0,  1.0, // blue
    -0.5,  0.5,  0.5, // face 3, vert 12
     1.0,  1.0,  0.0, // yellow
    -0.5, -0.5,  0.5, // face 3, vert 13
     1.0,  1.0,  0.0, // yellow
     0.5, -0.5,  0.5, // face 3, vert 14
     1.0,  1.0,  0.0, // yellow
     0.5,  0.5,  0.5, // face 3, vert 15
     1.0,  1.0,  0.0, // yellow
    -0.5,  0.5, -0.5, // face 4, vert 16
     1.0,  0.0,  1.0, // magenta
    -0.5,  0.5,  0.5, // face 4, vert 17
     1.0,  0.0,  1.0, // magenta
     0.5,  0.5,  0.5, // face 4, vert 18
     1.0,  0.0,  1.0, // magenta
     0.5,  0.5, -0.5, // face 4, vert 19
     1.0,  0.0,  1.0, // magenta
    -0.5, -0.5,  0.5, // face 5, vert 20
     0.0,  1.0,  1.0, // cyan
     0.5, -0.5,  0.5, // face 5, vert 21
     0.0,  1.0,  1.0, // cyan
     0.5, -0.5, -0.5, // face 5, vert 22
     0.0,  1.0,  1.0, // cyan
    -0.5, -0.5, -0.5, // face 5, vert 23
     0.0,  1.0,  1.0, // cyan
]);
const SIZE_OF_FLOAT = 4;
const NUM_COMPONENTS_POSITION = 3;
const NUM_COMPONENTS_COLOR = 3;
const VERTEX_STRIDE_BYTES = SIZE_OF_FLOAT * (NUM_COMPONENTS_POSITION + NUM_COMPONENTS_COLOR);

var gl;
var theProgram;
var triangleVertexBuffer;
var triangleIndexBuffer;
var rotationUniformLoc;

class Vec3 {
    constructor(x, y, z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }
    length() {
        return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
    }
    normalize() {
        var len = this.length();

        this.x /= len;
        this.y /= len;
        this.z /= len;
    }
}

class Mat4 {
    constructor() {
        this.m11 = 1.0;
        this.m21 = 0.0;
        this.m31 = 0.0;
        this.m41 = 0.0;

        this.m12 = 0.0;
        this.m22 = 1.0;
        this.m32 = 0.0;
        this.m42 = 0.0;

        this.m13 = 0.0;
        this.m23 = 0.0;
        this.m33 = 1.0;
        this.m43 = 0.0;

        this.m14 = 0.0;
        this.m24 = 0.0;
        this.m34 = 0.0;
        this.m44 = 1.0;
    }

    setRotation(normal, angle) {
        var angleCos;
        var angleSin;
        var oneMinusAngleCos;
        var normalXTimesNormalY;
        var normalXTimesNormalZ;
        var normalYTimesNormalZ;

        angleCos = Math.cos(angle);
        angleSin = Math.sin(angle);
        oneMinusAngleCos = 1.0 - angleCos;
        normalXTimesNormalY = normal.x * normal.y;
        normalXTimesNormalZ = normal.x * normal.z;
        normalYTimesNormalZ = normal.y * normal.z;

        this.m11 = normal.x * normal.x * oneMinusAngleCos + angleCos;
        this.m21 = normalXTimesNormalY * oneMinusAngleCos + normal.z * angleSin;
        this.m31 = normalXTimesNormalZ * oneMinusAngleCos - normal.y * angleSin;
        this.m41 = 0.0;

        this.m12 = normalXTimesNormalY * oneMinusAngleCos - normal.z * angleSin;
        this.m22 = normal.y * normal.y * oneMinusAngleCos + angleCos;
        this.m32 = normalYTimesNormalZ * oneMinusAngleCos + normal.x * angleSin;
        this.m42 = 0.0;

        this.m13 = normalXTimesNormalZ * oneMinusAngleCos + normal.y * angleSin;
        this.m23 = normalYTimesNormalZ * oneMinusAngleCos - normal.x * angleSin;
        this.m33 = normal.z * normal.z * oneMinusAngleCos + angleCos;
        this.m43 = 0.0;

        this.m14 = 0.0;
        this.m24 = 0.0;
        this.m34 = 0.0;
        this.m44 = 1.0;
    }
    
    toFloat32Array() {
        return new Float32Array([this.m11, this.m21, this.m31, this.m41,
                                 this.m12, this.m22, this.m32, this.m42,
                                 this.m13, this.m23, this.m33, this.m43,
                                 this.m14, this.m24, this.m34, this.m44]);
    }
}

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
    gl.bufferData(gl.ARRAY_BUFFER, CUBE_MESH, gl.STATIC_DRAW);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    triangleIndexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, CUBE_INDICES, gl.STATIC_DRAW);
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
    const ROTATION_AXIS = new Vec3(1.0, 1.0, 1.0);
    ROTATION_AXIS.normalize();
    const ROTATION_TIME_IN_MS = 5000.0;

    var nextRotationReset;
    var canvas = document.getElementById("glcanvas");
    var rotation = new Mat4();
    gl = initWebGL(canvas);
    if (!gl) {
        return;
    }

    initializeProgram(canvas);
    initializeTriangleBuffers();

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.enable(gl.DEPTH_TEST)
    gl.clearDepth(1.0);

    var draw = function(timeInMilliseconds) {
        
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        gl.useProgram(theProgram);

        if (timeInMilliseconds > nextRotationReset) {
            nextRotationReset = timeInMilliseconds + ROTATION_TIME_IN_MS;
        }
        var rotationPercentage = (nextRotationReset - timeInMilliseconds) / ROTATION_TIME_IN_MS;
        rotation.setRotation(ROTATION_AXIS, 2 * Math.PI * rotationPercentage);
        gl.uniformMatrix4fv(rotationUniformLoc, false, rotation.toFloat32Array());

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
        gl.drawElements(gl.TRIANGLES, CUBE_INDICES.length, gl.UNSIGNED_SHORT, 0);
        gl.disableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
        gl.disableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);

        gl.useProgram(null);

        window.requestAnimationFrame(draw);
    };
    var firstDraw = function (timeInMilliseconds) {
        nextRotationReset = timeInMilliseconds;
        draw(timeInMilliseconds);
    };
    window.requestAnimationFrame(firstDraw);
}
