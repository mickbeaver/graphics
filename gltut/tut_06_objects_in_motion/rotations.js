const VERTEX_SHADER_SRC = `#version 100
    uniform mat4 u_cameraToClipMatrix;
    uniform mat4 u_modelToCameraMatrix;

    attribute vec4 a_position;
    attribute vec4 a_color;
    varying vec4 v_color;

    void
    main()
    {
        vec4 cameraPos = u_modelToCameraMatrix * a_position;
        gl_Position = u_cameraToClipMatrix * cameraPos;
        v_color = a_color;
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
const MESH_VERTEX_STREAMS = new Float32Array([
    +1.0, +1.0, +1.0,
    -1.0, -1.0, +1.0,
    -1.0, +1.0, -1.0,
    +1.0, -1.0, -1.0,

    -1.0, -1.0, -1.0,
    +1.0, +1.0, -1.0,
    +1.0, -1.0, +1.0,
    -1.0, +1.0, +1.0,

    0.0, 1.0, 0.0, 1.0, // green
    0.0, 0.0, 1.0, 1.0, // blue
    1.0, 0.0, 0.0, 1.0, // red
    0.5, 0.5, 0.0, 1.0, // brown

    0.0, 1.0, 0.0, 1.0, // green
    0.0, 0.0, 1.0, 1.0, // blue
    1.0, 0.0, 0.0, 1.0, // red
    0.5, 0.5, 0.0, 1.0, // brown
]);
const MESH_INDICES = new Uint16Array([
    0, 1, 2,
    1, 0, 3,
    2, 3, 0,
    3, 2, 1,

    5, 4, 6,
    4, 5, 7,
    7, 6, 4,
    6, 7, 5,
]);
const MESH_NUM_VERTICES = 8;
const MESH_NUM_COMPONENTS_POSITION = 3;
const MESH_NUM_COMPONENTS_COLOR = 4;

var gl;
var theProgram;
var triangleVertexBuffer;
var triangleIndexBuffer;
var modelToCameraMatrixUnif;

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

    setTranslation(x, y, z) {
        this.m14 = x;
        this.m24 = y;
        this.m34 = z;
    }

    setPerspective(right, top, near, far) {
        // Assumes symmetric view where right == -left and top == -bottom
        this.m11 = near / right;
        this.m21 = 0.0;
        this.m31 = 0.0;
        this.m41 = 0.0;

        this.m12 = 0.0;
        this.m22 = near / top;
        this.m32 = 0.0;
        this.m42 = 0.0;

        this.m13 = 0.0;
        this.m23 = 0.0;
        this.m33 = (far + near) / (near - far);
        this.m43 = -1.0;

        this.m14 = 0.0;
        this.m24 = 0.0;
        this.m34 = (2 * far * near) / (near - far);
        this.m44 = 0.0;
    }

    setFovPerspective(verticalFovDegrees, aspectRatio, near, far) {
        // Assumes symmetric view where right == -left and top == -bottom
        var right;
        var top;

        top = near * Math.tan(verticalFovDegrees * Math.PI / 360.0);
        right = top * aspectRatio;

        this.setPerspective(right, top, near, far);
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

    setScale(scale) {
        this.m11 = scale;
        this.m22 = scale;
        this.m33 = scale;
    }
    
    copy(other) {
        this.m11 = other.m11;
        this.m21 = other.m21;
        this.m31 = other.m31;
        this.m41 = other.m41;

        this.m12 = other.m12;
        this.m22 = other.m22;
        this.m32 = other.m32;
        this.m42 = other.m42;

        this.m13 = other.m13;
        this.m23 = other.m23;
        this.m33 = other.m33;
        this.m43 = other.m43;

        this.m14 = other.m14;
        this.m24 = other.m24;
        this.m34 = other.m34;
        this.m44 = other.m44;
    }

    leftMultiply(other) {
        var temp = new Mat4();
        temp.m11 = ((this.m11 * other.m11) +
                    (this.m12 * other.m21) +
                    (this.m13 * other.m31) +
                    (this.m14 * other.m41));
        temp.m21 = ((this.m21 * other.m11) +
                    (this.m22 * other.m21) +
                    (this.m23 * other.m31) +
                    (this.m24 * other.m41));
        temp.m31 = ((this.m31 * other.m11) +
                    (this.m32 * other.m21) +
                    (this.m33 * other.m31) +
                    (this.m34 * other.m41));
        temp.m41 = ((this.m41 * other.m11) +
                    (this.m42 * other.m21) +
                    (this.m43 * other.m31) +
                    (this.m44 * other.m41));
        temp.m12 = ((this.m11 * other.m12) +
                    (this.m12 * other.m22) +
                    (this.m13 * other.m32) +
                    (this.m14 * other.m42));
        temp.m22 = ((this.m21 * other.m12) +
                    (this.m22 * other.m22) +
                    (this.m23 * other.m32) +
                    (this.m24 * other.m42));
        temp.m32 = ((this.m31 * other.m12) +
                    (this.m32 * other.m22) +
                    (this.m33 * other.m32) +
                    (this.m34 * other.m42));
        temp.m42 = ((this.m41 * other.m12) +
                    (this.m42 * other.m22) +
                    (this.m43 * other.m32) +
                    (this.m44 * other.m42));
        temp.m13 = ((this.m11 * other.m13) +
                    (this.m12 * other.m23) +
                    (this.m13 * other.m33) +
                    (this.m14 * other.m43));
        temp.m23 = ((this.m21 * other.m13) +
                    (this.m22 * other.m23) +
                    (this.m23 * other.m33) +
                    (this.m24 * other.m43));
        temp.m33 = ((this.m31 * other.m13) +
                    (this.m32 * other.m23) +
                    (this.m33 * other.m33) +
                    (this.m34 * other.m43));
        temp.m43 = ((this.m41 * other.m13) +
                    (this.m42 * other.m23) +
                    (this.m43 * other.m33) +
                    (this.m44 * other.m43));
        temp.m14 = ((this.m11 * other.m14) +
                    (this.m12 * other.m24) +
                    (this.m13 * other.m34) +
                    (this.m14 * other.m44));
        temp.m24 = ((this.m21 * other.m14) +
                    (this.m22 * other.m24) +
                    (this.m23 * other.m34) +
                    (this.m24 * other.m44));
        temp.m34 = ((this.m31 * other.m14) +
                    (this.m32 * other.m24) +
                    (this.m33 * other.m34) +
                    (this.m34 * other.m44));
        temp.m44 = ((this.m41 * other.m14) +
                    (this.m42 * other.m24) +
                    (this.m43 * other.m34) +
                    (this.m44 * other.m44));
        this.copy(temp);

        return this;
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
    const Z_NEAR = 1.0;
    const Z_FAR = 45.0;
    const VERTICAL_FOV_DEGREES = 45.0;

    var vertexShader = createShader(gl.VERTEX_SHADER, VERTEX_SHADER_SRC);
    var fragmentShader = createShader(gl.FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);
    theProgram = createProgram(vertexShader, fragmentShader);
    if (theProgram) {
        let cameraToClipMatrixUnif = gl.getUniformLocation(theProgram, "u_cameraToClipMatrix");
        modelToCameraMatrixUnif = gl.getUniformLocation(theProgram, "u_modelToCameraMatrix");

        let cameraToClipMatrix = new Mat4();
        let aspectRatio = canvas.width / canvas.height;
        cameraToClipMatrix.setFovPerspective(VERTICAL_FOV_DEGREES, aspectRatio, Z_NEAR, Z_FAR);
        gl.useProgram(theProgram);
        gl.uniformMatrix4fv(cameraToClipMatrixUnif, false, cameraToClipMatrix.toFloat32Array());
        gl.useProgram(null);
    }
    
    gl.deleteShader(vertexShader);
    gl.deleteShader(fragmentShader);
}

function initializeTriangleBuffers() {
    triangleVertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, triangleVertexBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, MESH_VERTEX_STREAMS, gl.STATIC_DRAW);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    triangleIndexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, MESH_INDICES, gl.STATIC_DRAW);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, null);
}

function initWebGL(canvas) {
    var context = canvas.getContext("webgl");
    if (!context) {
        alert("Unable to initialize WebGL. Your browser may not support it.");
    }

    return context;
}

function nullRotation(elapsedTime) {
    var modelTransform = new Mat4();

    modelTransform.m34 = -25.0;

    return modelTransform;
}

function computeAngleRadians(elapsedTime, loopDuration) {
    const scale = Math.PI * 2.0 / loopDuration;

    return (elapsedTime % loopDuration) * scale;
}

function rotateX(elapsedTime) {
    const angleRadians = computeAngleRadians(elapsedTime, 3.0);
    const angleCos = Math.cos(angleRadians);
    const angleSin = Math.sin(angleRadians);

    var modelTransform = new Mat4();
    modelTransform.m22 = angleCos;
    modelTransform.m23 = -angleSin;
    modelTransform.m32 = angleSin;
    modelTransform.m33 = angleCos;

    modelTransform.m14 = -5.0;
    modelTransform.m24 = -5.0;
    modelTransform.m34 = -25.0;
    
    return modelTransform;
}

function rotateY(elapsedTime) {
    const angleRadians = computeAngleRadians(elapsedTime, 2.0);
    const angleCos = Math.cos(angleRadians);
    const angleSin = Math.sin(angleRadians);

    var modelTransform = new Mat4();
    modelTransform.m11 = angleCos;
    modelTransform.m13 = angleSin;
    modelTransform.m31 = -angleSin;
    modelTransform.m33 = angleCos;

    modelTransform.m14 = -5.0;
    modelTransform.m24 = 5.0;
    modelTransform.m34 = -25.0;
    
    return modelTransform;
}

function rotateZ(elapsedTime) {
    const angleRadians = computeAngleRadians(elapsedTime, 2.0);
    const angleCos = Math.cos(angleRadians);
    const angleSin = Math.sin(angleRadians);

    var modelTransform = new Mat4();
    modelTransform.m11 = angleCos;
    modelTransform.m12 = -angleSin;
    modelTransform.m21 = angleSin;
    modelTransform.m22 = angleCos;

    modelTransform.m14 = 5.0;
    modelTransform.m24 = 5.0;
    modelTransform.m34 = -25.0;
    
    return modelTransform;
}

function rotateAxis(elapsedTime) {
    const angleRadians = computeAngleRadians(elapsedTime, 2.0);
    var rotationAxis = new Vec3(1.0, 1.0, 1.0);
    rotationAxis.normalize();

    var modelTransform = new Mat4();
    modelTransform.setRotation(rotationAxis, angleRadians);

    modelTransform.m14 = 5.0;
    modelTransform.m24 = -5.0;
    modelTransform.m34 = -25.0;

    return modelTransform;
}

function start() {
    var canvas = document.getElementById("glcanvas");
    const INSTANCE_FUNCS = [nullRotation, rotateX, rotateY, rotateZ, rotateAxis];
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

        gl.bindBuffer(gl.ARRAY_BUFFER, triangleVertexBuffer);
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
        gl.enableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
        gl.enableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);
        gl.vertexAttribPointer(VERTEX_ATTR_INDEX_POSITION,
                               MESH_NUM_COMPONENTS_POSITION,
                               gl.FLOAT,
                               false,
                               0,
                               0);
        gl.vertexAttribPointer(VERTEX_ATTR_INDEX_COLOR,
                               MESH_NUM_COMPONENTS_COLOR,
                               gl.FLOAT,
                               false,
                               0,
                               Float32Array.BYTES_PER_ELEMENT * MESH_NUM_COMPONENTS_POSITION * MESH_NUM_VERTICES);
        for (let instanceFunc of INSTANCE_FUNCS) {
            let modelToCameraMatrix = instanceFunc(timeInMilliseconds / 1000.0);

            gl.uniformMatrix4fv(modelToCameraMatrixUnif, false, modelToCameraMatrix.toFloat32Array());
            gl.drawElements(gl.TRIANGLES, MESH_INDICES.length, gl.UNSIGNED_SHORT, 0);
        }

        gl.disableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
        gl.disableVertexAttribArray(VERTEX_ATTR_INDEX_COLOR);

        gl.useProgram(null);

        window.requestAnimationFrame(draw);
    };
    window.requestAnimationFrame(draw);
}
