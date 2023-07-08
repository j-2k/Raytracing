# First Raytracing Project

This is my first time trying to tackle raytracing from scratch, I will be documenting my learning journey on everything I do & learn on my website. I try to make it in a way that is simple to understand as much as possible, if you would like to check it out click the blog link above or click [here](https://j-2k.github.io/raytracingproject).

Its important to know before hand this project is not fully written from "scratch" but it kind of is atleast the raytracing part. we are mostly using a vulkan image & assigning its pixels to "emulate" a raytracer, but anyway this project is a [Walnut](https://github.com/TheCherno/Walnut) template originally & we just built a raytracer ontop of it. Its mostly just ImGUI for UI, GLM for math, & GLFW for window stuff.

I also show how this raytracer works in unity & show the individual rays of how they create a sphere, this is essentially the same algorithim used in my raytracer. To see visualization & images of how my raytracer works you can check either the [Repository](https://github.com/j-2k/VisualizeRaytracingInUnity) or [Blog](https://j-2k.github.io/raytracingproject/#Visualize_Raytracing).<br />


# Building this project

I used Visual Studio 2022 so I suggest that IDE, but you also need latest version of the Vulkan SDK.

## Learning Resources & Useful Links
- [Visualizing this raytracer in unity](https://github.com/j-2k/VisualizeRaytracingInUnity) Check this out if you are interested in how I visualize THIS raytracer in unity!
- [Cherno Raytracing Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl) Main guide I follow
- [Raytracing in 1 Weekend](https://raytracing.github.io) Infamous book for raytracing
- [Scratchapixel](https://scratchapixel.com) Learn computer graphics topics & all math prerequisites.
- [ssloy](https://github.com/ssloy) More guides about CG topics from ssloy
- [My Website documenting this whole project.](https://j-2k.github.io) Check it out if you are interested!

## My journey through raytracing with pictures!
Click [here](https://j-2k.github.io/raytracingproject) to go to my website where I show much more in a better fashion.

#### The Hello World Stage
![first uv](https://user-images.githubusercontent.com/52252068/234472575-81e72086-39fe-4f46-8550-3d80f62e45a1.png)

#### The first sphere using ray intersection
![first sphere](https://user-images.githubusercontent.com/52252068/234472603-626bd8ee-0446-4308-a1e9-c7b069c9da7b.png)

#### Visualizing the raytracer in Unity
![Visualkization](https://user-images.githubusercontent.com/52252068/234472677-a8b79507-5bcf-42e5-93e9-5931b121fa63.gif)

#### First shaded raytraced sphere
Camera interactivity was also added.
![Screenshot_2](https://user-images.githubusercontent.com/52252068/234472619-360c6518-8d1f-481a-8153-7521b2461c02.png)

#### Multiple Spheres & Reflective rays added
![reflective](https://github.com/j-2k/Raytracing/assets/52252068/dfb3f6b4-0bd4-43a7-a8db-033821ef65ce)

#### Roughness "Material" (only applied on bottom sphere)
![roughness](https://github.com/j-2k/Raytracing/assets/52252068/927f2439-335d-4b56-8358-a69ccbca2f49)

#### Skybox color added
![roughsky](https://github.com/j-2k/Raytracing/assets/52252068/42e3bc86-94ae-42e5-897b-eb563b38a3f3)

#### Visualizing Ambient Occlusion with different roughness
![ao no rough](https://github.com/j-2k/Raytracing/assets/52252068/19bd31cf-0ff1-4f23-a3cc-dbf9d6629dd6)
![AOro](https://github.com/j-2k/Raytracing/assets/52252068/37ef9ab5-c7e3-499c-9877-542ef7fc772d)
![ao rough](https://github.com/j-2k/Raytracing/assets/52252068/27b0c780-33b6-4fc2-8f42-eed5c70de533)

#### Roughness + Skybox + AO 
![nice](https://github.com/j-2k/Raytracing/assets/52252068/2107a1ba-c9f1-4c71-8024-5fb134e5d5d0)

#### Pathtracing (Accumulation of frames)
![ptrt](https://github.com/j-2k/Raytracing/assets/52252068/f2a9fd6b-e92f-4c3f-a738-40b89fa3953f)

#### CPU Multithreading 
##### Performance Increase: 76% Faster Overall (50ms > 20ms > 12 ms)
*60% Faster 50ms to 20ms* - Utilizing all cores using for each function with a parallel execution parameter.  
*40% Faster 20ms to 12ms*- All cores were using 1 Random Engine, Changed to use thread_local, so each thread has its own random engine.  
![CPU Multithreading](https://github.com/j-2k/Raytracing/assets/52252068/582ab8ac-2fe1-4b43-b2ce-64a613695599)

#### Emissions
![Screenshot_4](https://github.com/j-2k/Raytracing/assets/52252068/7addef7e-4cd9-4829-9e21-4e3a8706ca0e)
![Screenshot_5](https://github.com/j-2k/Raytracing/assets/52252068/c3397570-7858-42cb-ba0b-3278be5a0320)
![Screenshot_6](https://github.com/j-2k/Raytracing/assets/52252068/47aad42d-355f-420e-b99f-1696cdd9e376)
![Screenshot_7](https://github.com/j-2k/Raytracing/assets/52252068/d958fff5-9c8f-44bf-b79b-debae131c059)
![Screenshot_8](https://github.com/j-2k/Raytracing/assets/52252068/a339c4bb-0a3e-406c-a74f-9b2cbcc993b6)
![Screenshot_9](https://github.com/j-2k/Raytracing/assets/52252068/5ab842b2-0a53-4d53-9903-897131818681)




