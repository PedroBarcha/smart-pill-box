# Smart Pill Box
[![](http://res.cloudinary.com/dkxkt5ibp/image/upload/v1521334400/queijino.png)](https://www.youtube.com/watch?v=uawi_aQo4Ek)

A pill box that reminds the user to take her/his pills, without requiring any time programming (through buttons or a phone app, for example), which is challenging for old people. All that the person has to do is to put the pills in one of the slices (blue, pink and purple in the figure bellow) and then put it into the corresponding hour case of the box.

<p align="center">
 <img src="http://res.cloudinary.com/dkxkt5ibp/image/upload/v1521332405/pill-box.png" width="75%" height="75%">
</p>

The case has 12 colorful slices, each one corresponding to one hour. The arduino with its components is inside the grey part of the box.

# Arduino Sketch
Components:
* Real Time Clock: RTC DS3231
* Stepper Motor: BYJ48
* LED Lamp
* Resistors: 12 x 220ohms
 
<p align="center">
 <img src="http://res.cloudinary.com/dkxkt5ibp/image/upload/v1521331734/pills_sketch_uno.png" width="65%" height="65%">
</p>

Bellow each one of the 12 colorful slices there is a thin aluminium plate that touches a wire, which closes the circuit for that specific hour. Each one of these wires should be connected right next to the resistors in the figure above, next to the red line of the breadboard.

# Pill Box Template
The pill box was modeled by Emilia Finardi and Federica Rho (both from Politecnico di Milano) and stamped with a 3D stamper. If you want the box template files, email me (correia.barcha at gmail.com) and I shall ask their permission to share the files.

# Pseudo-Algorithm
<p align="center">
  <img src="http://res.cloudinary.com/dkxkt5ibp/image/upload/v1521331483/pseudo-algorithm-final.png">
</p>
