# File: Moon.py
# Description: A program modeling the restricted three body program of Sun, Earth, Moon
from vpython import *

# simulation of the motion of the earth around the sun
def main():
	
	# simulation of the motion of the earth around the sun
	G = 6.674e-11

	# mass of sun
	m_sun = 1.9891e30

	# radius of the sun (not to scale)
	#r_sun_actual = 6.9634e8
	r_sun = 5e10

	# mass of earth
	m_earth = 6.972e24

	# radius of earth (not to scale)
	#r_earth_actual = 6.367e6
	r_earth = 1e10

	# radius of moon not to scale
	r_moon = 1e10

	# mass of moon
	m_moon = 7.35e22

	# create the objects
	sun_obj = sphere(pos=vector(0, 0, 0), radius=r_sun, color=color.yellow)
	earth_obj = sphere(pos=vector(1.496e11, 0, 0), radius=r_earth, color=color.blue, make_trail=True)
	moon_obj = sphere(pos=vector(1.496e11+3.84e8,0,0), radius=r_moon, color=color.white, make_trail=True)
	#circle_vel = sqrt(G*mass_sun/mag(r))
	earth_obj.velocity = vector(0, 3e4, 0)
	moon_obj.velocity = vector(0, sqrt(G*m_earth/mag(moon_obj.pos-earth_obj.pos))+3e4,0)

	# define initial time
	t = 0

	# time interval
	dt = 3600
	

	while t < 1e10:
		rate(300)
		earth_obj.pos = earth_obj.pos + earth_obj.velocity*dt
		moon_obj.pos = moon_obj.pos + moon_obj.velocity*dt


		es_r_vector = earth_obj.pos - sun_obj.pos
		em_r_vector = moon_obj.pos - earth_obj.pos
		ms_r_vector = moon_obj.pos - sun_obj.pos

		F_grav_e = norm(es_r_vector)*(-G*m_sun*m_earth /mag(es_r_vector)**2) + -1*norm(em_r_vector)*(-G*m_earth*m_moon/mag(em_r_vector)**2)
		F_grav_m = norm(ms_r_vector)*(-G*m_sun*m_moon/mag(ms_r_vector)**2) + norm(em_r_vector)*(-G*m_earth*m_moon/mag(em_r_vector)**2)

		earth_obj.velocity = earth_obj.velocity + (F_grav_e / m_earth)*dt
		moon_obj.velocity = moon_obj.velocity + (F_grav_m / m_moon)*dt
		#print(earth_obj.pos, F_grav, earth_obj.velocity, norm(r_vector))
		t += dt


main()

