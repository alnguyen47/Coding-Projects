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

	# create the objects
	sun_obj = sphere(pos=vector(0, 0, 0), radius=r_sun, color=color.yellow)
	earth_obj = sphere(pos=vector(1.496e11, 0, 0), radius=r_earth, color=color.blue, make_trail=True)

	#circle_vel = sqrt(G*mass_sun/mag(r))
	earth_obj.velocity = vector(0, 3e4, 0)

	# define initial time
	t = 0

	# time interval
	dt = 3600
	

	while t < 1e10:
		rate(300)
		earth_obj.pos = earth_obj.pos + earth_obj.velocity*dt
		r_vector = earth_obj.pos - sun_obj.pos
		F_grav = norm(r_vector)*(-G*m_sun*m_earth / (mag(r_vector)**2)) 
		earth_obj.velocity = earth_obj.velocity + (F_grav / m_earth)*dt
		#print(earth_obj.pos, F_grav, earth_obj.velocity, norm(r_vector))
		t += dt


main()

