const VERTEX_SHADER_SRC = `#version 100
    uniform mat4 u_modelTransform;
    uniform mat4 u_projection;
    attribute vec4 a_position;
    attribute vec4 a_color;
    varying vec4 v_color;

    void
    main()
    {
        v_color = a_color;
        gl_Position = u_projection * u_modelTransform * a_position;
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
const CUBE_INDICES = new Uint16Array([0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23]);
const CUBE_MESH = new Float32Array([
    1.0, 1.0, 1.0,
     1.0,  0.0,  0.0, // red
    -1.0, 1.0, 1.0,
     1.0,  0.0,  0.0, // red
    -1.0, -1.0, 1.0,
     1.0,  0.0,  0.0, // red
    1.0, -1.0, 1.0,
     1.0,  0.0,  0.0, // red
    1.0, 1.0, -1.0,
     0.0,  1.0,  0.0, // green
    1.0, 1.0, 1.0,
     0.0,  1.0,  0.0, // green
    1.0, -1.0, 1.0,
     0.0,  1.0,  0.0, // green
    1.0, -1.0, -1.0,
     0.0,  1.0,  0.0, // green
    -1.0, 1.0, -1.0,
     0.0,  0.0,  1.0, // blue
    1.0, 1.0, -1.0,
     0.0,  0.0,  1.0, // blue
    1.0, -1.0, -1.0,
     0.0,  0.0,  1.0, // blue
    -1.0, -1.0, -1.0,
     0.0,  0.0,  1.0, // blue
    -1.0, 1.0, 1.0,
    1.0,  1.0,  0.0, // yellow
    -1.0, 1.0, -1.0,
     1.0,  1.0,  0.0, // yellow
    -1.0, -1.0, -1.0,
     1.0,  1.0,  0.0, // yellow
    -1.0, -1.0, 1.0,
     1.0,  1.0,  0.0, // yellow
    1.0, 1.0, -1.0,
     1.0,  0.0,  1.0, // magenta
    -1.0, 1.0, -1.0,
     1.0,  0.0,  1.0, // magenta
    -1.0, 1.0, 1.0,
     1.0,  0.0,  1.0, // magenta
    1.0, 1.0, 1.0,
     1.0,  0.0,  1.0, // magenta
    1.0, -1.0, 1.0,
     0.0,  1.0,  1.0, // cyan
    -1.0, -1.0, 1.0,
     0.0,  1.0,  1.0, // cyan
    -1.0, -1.0, -1.0,
     0.0,  1.0,  1.0, // cyan
    1.0, -1.0, -1.0,
     0.0,  1.0,  1.0, // cyan
]);
const NUM_COMPONENTS_POSITION = 3;
const NUM_COMPONENTS_COLOR = 3;
const VERTEX_STRIDE_BYTES = Float32Array.BYTES_PER_ELEMENT * (NUM_COMPONENTS_POSITION + NUM_COMPONENTS_COLOR);

var gl;
var theProgram;
var triangleVertexBuffer;
var triangleIndexBuffer;
var modelTransformUniformLoc;
var projectionUniformLoc;

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

    setPerspective(width, height, near, far) {
        this.m11 = near / (width / 2.0);
        this.m21 = 0.0;
        this.m31 = 0.0;
        this.m41 = 0.0;

        this.m12 = 0.0;
        this.m22 = near / (height / 2.0);
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

    setOrtho(width, height, near, far) {
        this.m11 = 1.0 / (width / 2.0);
        this.m21 = 0.0;
        this.m31 = 0.0;
        this.m41 = 0.0;

        this.m12 = 0.0;
        this.m22 = 1 / (height / 2.0);
        this.m32 = 0.0;
        this.m42 = 0.0;

        this.m13 = 0.0;
        this.m23 = 0.0;
        this.m33 = 2.0 / (near - far);
        this.m43 = 0.0;

        this.m14 = 0.0;
        this.m24 = 0.0;
        this.m34 = (near + far) / (near - far);
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
    var vertexShader;
    var fragmentShader;

    vertexShader = createShader(gl.VERTEX_SHADER, VERTEX_SHADER_SRC);
    fragmentShader = createShader(gl.FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);
    theProgram = createProgram(vertexShader, fragmentShader);
    if (theProgram) {
        modelTransformUniformLoc = gl.getUniformLocation(theProgram, "u_modelTransform");
        projectionUniformLoc = gl.getUniformLocation(theProgram, "u_projection");
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
    const ROTATION_TIME_IN_MS = 5000.0;
    const NEAR_PLANE = {width: 2.0, height: 2.0};
    const Z_NEAR = 0.5;
    const Z_FAR = 5.0;

    var nextRotationReset;
    var rotationAxis = new Vec3(1.0, 0.0, 0.0);
    var canvas = document.getElementById("glcanvas");
    gl = initWebGL(canvas);
    if (!gl) {
        return;
    }

    initializeProgram(canvas);
    initializeTriangleBuffers();

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.enable(gl.DEPTH_TEST)
    gl.clearDepth(1.0);
    gl.enable(gl.CULL_FACE);
    gl.cullFace(gl.BACK);

    var draw = function(timeInMilliseconds) {
        
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        gl.useProgram(theProgram);

        var projection = new Mat4();
        projection.setPerspective(NEAR_PLANE.width, NEAR_PLANE.height, Z_NEAR, Z_FAR);
        gl.uniformMatrix4fv(projectionUniformLoc, false, projection.toFloat32Array());

        var rotation = new Mat4();
        var scale = new Mat4();
        var translation = new Mat4();
        
        if (timeInMilliseconds > nextRotationReset) {
            nextRotationReset = timeInMilliseconds + ROTATION_TIME_IN_MS;
            if (rotationAxis.x == 1.0) {
                rotationAxis = new Vec3(0.0, 1.0, 0.0);
            } else if (rotationAxis.y == 1.0) {
                rotationAxis = new Vec3(1.0, 1.0, 1.0);
                rotationAxis.normalize();
            } else {
                rotationAxis = new Vec3(1.0, 0.0, 0.0);
            }
        }
        var rotationPercentage = (nextRotationReset - timeInMilliseconds) / ROTATION_TIME_IN_MS;

        rotation.setRotation(rotationAxis, 2 * Math.PI * rotationPercentage);
        //scale.setScale(1.0);
        translation.setTranslation(0.0, 0.0, -3.0);
        var modelTransform = translation.leftMultiply(rotation.leftMultiply(scale));
        gl.uniformMatrix4fv(modelTransformUniformLoc, false, modelTransform.toFloat32Array());

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
                               Float32Array.BYTES_PER_ELEMENT * NUM_COMPONENTS_POSITION);
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
