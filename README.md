# N-Body Problem Simulation

An interactive N-body gravity simulation with 2D physics and 3D rendering, featuring multiple force calculation algorithms and numerical integrators.

## Features

### Physics Engine
- **Realistic Units**: AU (distance), Solar Masses (mass), Years (time)
- **Gravitational Constant**: 39.478 AU³/(M☉·year²)
- **Multiple Integrators**: Euler and Verlet
- **Force Calculation Methods**:
  - Direct pairwise O(N²)
  - Barnes-Hut tree O(N log N)
  - Fast Multipole Method O(N) (Not yet implemented)
- **Stable Timestep**: Internal physics sub-stepping for stability, default 0.0001 year.

### Rendering
- **OpenGL 3.3 Core**: Modern OpenGL with GLAD and GLFW
- **3D Perspective Camera**: Spherical coordinate system
- **Gravitational Field Visualization**: Grid showing field strength
- **Mass Object Rendering**: Spheres with size/color based on mass
- **ImGui Interface**: Real-time controls and algorithm selection

### Example Solar System
- Sun, Earth, Mars, Jupiter, Ganymede, and an asteroid
- Realistic orbital velocities for circular orbits
- Proper mass ratios and distances

## Project Structure

```
src/
├── main.cpp                    # Application entry point, OpenGL setup, render loop
├── PhysicsEngine.h/.cpp        # Core physics engine with timestep management
├── ForceCalculator.h/.cpp      # BruteForceCalculator implementation
├── BarnesHutCalculator.h/.cpp  # Barnes-Hut algorithm implementation
├── Integrator.h/.cpp           # Euler and Verlet integrators
├── MassObject.h/.cpp           # Mass object with position, velocity, acceleration
├── MassObjectTracker.h/.cpp    # Simulation state management
├── GridGenerator.h/.cpp        # Gravitational field grid visualization
├── Sphere.h/.cpp               # 3D sphere geometry
├── SphereRenderer.h/.cpp       # Sphere rendering system
├── Camera.h/.cpp               # 3D camera with spherical coordinates
└── vertex.glsl, fragment.glsl  # Shader files

include/                        # Header dependencies
├── glm/                        # OpenGL Mathematics library
├── imgui/                      # Dear ImGui for UI
└── glad/                       # OpenGL loader

libs/                           # Pre-built libraries
└── glfw-3.4.bin.WIN64/        # GLFW binaries for Windows

```


## Controls

### User Interface
- **Time Speed Slider**: Adjust simulation speed (0.1x to 10x)
- **Algorithm Selection**: Choose between force calculation methods

### Keyboard Controls
- `SPACE`: Toggle physics simulation on/off
- `E`: Switch to Euler integrator
- `V`: Switch to Velocity Verlet integrator
- `W/A/S/D`: Rotate camera (spherical coordinates)
- `Arrow Keys`: Move camera
- `ESC`: Exit application

## Configuration

### Physics Parameters
```cpp
// Set physics timestep
massTracker.getPhysicsEngine().setPhysicsTimestep(0.0001);

// Switch force calculation methods
massTracker.switchToBruteForce(double G, double softening);
massTracker.switchToBarnesHut(double G, double theta, double softening);

// Switch integrators
massTracker.switchToEulerIntegrator();
massTracker.switchToVerletIntegrator();
```

### Adding Mass Objects
```cpp
// Mass in M☉ (solar masses), position in AU, velocity in AU/year
massTracker.addMassObject(MassObject(1.0, {0.0f, 0.0f}, {0.0f, 0.0f})); // Sun
massTracker.addMassObject(MassObject(3e-6, {1.0f, 0.0f}, {0.0f, 6.28f})); // Earth
```

## Energy Conservation

The simulation monitors energy conservation and reports energy drift every 2 simulated seconds:
- `PhysicsEngine::calculateTotalEnergy()` calculates total system energy
- Useful for verifying integrator stability and timestep choice

## Dependencies

All dependencies are included in the repository:
- **GLFW 3.4**: Window management and input handling
- **GLAD**: OpenGL function loading
- **GLM**: OpenGL Mathematics library
- **Dear ImGui**: Immediate mode GUI

## Future Enhancements

- [ ] Implement Fast Multipole Method
- [ ] Add reset functionality
- [ ] Example Galaxy Scene
- [ ] Scene presets and save/load
- [ ] Performance profiling tools


## Author

Created by Batu Eren
