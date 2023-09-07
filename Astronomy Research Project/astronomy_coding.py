from astropy.coordinates import SkyCoord  # High-level coordinates
from astropy import units as u
from sys import stdin
import pandas as pd
import math
import plotly.express as px
import numpy as np
from plotly.subplots import make_subplots

def parse_parallax(x):
	if x == '' or float(x) <= 0:
		return 0
	elif float(x) != '' and float(x) > 0:
		return 1/float(x)
	else:
		return math.inf

def change_cartesian(RA, DEC, Dist):
	# Convert arcseconds to rad
	arc2Rad = np.pi/(3600*180)
	
	celesx = Dist*np.sin(np.pi/2 - DEC*arc2Rad)*np.cos(RA*arc2Rad)
	celesy = Dist*np.sin(np.pi/2 - DEC*arc2Rad)*np.sin(RA*arc2Rad)
	celesz = Dist*np.cos(np.pi/2 - DEC*arc2Rad)

	return [celesx, celesy, celesz]

def galactic_longlat(RA, DEC):
	# Get the galactic
	c = SkyCoord(ra=RA*u.arcsecond, dec=DEC*u.arcsecond)
	c = c.galactic
	lGal = c.l.value
	bGal = c.b.value

	return [lGal, bGal]


def str_to_numbers(x):
	RA = x * 3600 *15 #arcseconds
	#RA = (int(x[0:2]) + int(x[2:4])/60 + float(x[4:8])/3600) * 15 # degrees
	return RA

def str_to_numbers2(y):
	return y*3600 
		#return int(x[1:3]) + int(x[3:5])/60 + int(x[5:7]/3600)

def plot_3D(df):
	color_dict = {"O": 'blue', "B": 'blue', "A": 'blue', "F": 'yellow', "G": 'yellow', "K": 'red', "M": 'red'}
	fig = px.scatter_3d(df, x='Cartesian X', y='Cartesian Y', z='Cartesian Z', color="Spectral Code", color_discrete_map=color_dict,
		title="3D Plot of OBAFGKM Stars")
	return fig

def plot_2D(df):
	color_dict = {"O": 'blue', "B": 'blue', "A": 'blue', "F": 'yellow', "G": 'yellow', "K": 'red', "M": 'red'}

	fig1 = px.scatter(df, x='RA-DEC XProjXY', y='RA-DEC YProjXY', color="Spectral Code", color_discrete_map=color_dict, labels= {'RA-DEC XProjXY': 'x', 'RA-DEC YProjXY': 'y'}, title="XY Projection of Stars With Equatorial Coordinates")
	fig2 = px.scatter(df, x='RA-DEC XProjXZ', y='RA-DEC ZProjXZ', color="Spectral Code", color_discrete_map=color_dict, labels= {'RA-DEC XProjXZ': 'x', 'RA-DEC ZProjXZ': 'z'}, title="XZ Projection of Stars With Equatorial Coordinates")
	fig3 = px.scatter(df, x='RA-DEC YProjYZ', y='RA-DEC ZProjYZ', color="Spectral Code", color_discrete_map=color_dict, labels= {'RA-DEC YProjYZ': 'y', 'RA-DEC ZProjYZ': 'z'}, title="YZ Projection of Stars With Equatorial Coordinates")
	fig4 = px.scatter(df, x='Galactic XProjXY', y='Galactic YProjXY', color="Spectral Code", color_discrete_map=color_dict, labels= {'Galactic XProjXY': 'x', 'Galactic YProjXY': 'y'}, title="XY Projection of Stars With Galactic Coordinates")
	fig5 = px.scatter(df, x='Galactic XProjXZ', y='Galactic ZProjXZ', color="Spectral Code", color_discrete_map=color_dict, labels= {'Galactic XProjXZ': 'x', 'Galactic ZProjXZ': 'z'}, title="XZ Projection of Stars With Galactic Coordinates")
	fig6 = px.scatter(df, x='Galactic YProjYZ', y='Galactic ZProjYZ', color="Spectral Code", color_discrete_map=color_dict, labels= {'Galactic YProjYZ': 'y', 'Galactic ZProjYZ': 'z'}, title="YZ Projection of Stars With Galactic Coordinates")
	return [fig1, fig2, fig3, fig4, fig5, fig6]

def Coord_Proj(val1, val2, ind):
	arc2Rad = np.pi/(3600*180) # Conversion to radians from arcsecond
	deg2Rad = np.pi/180
	
	if ind == 'RA_DEC':
		phi = val1*arc2Rad
		lambdA = val2*arc2Rad
	elif ind == 'Galactic':
		phi = val1*deg2Rad
		lambdA = val2*deg2Rad
	
	x_proj = (2*np.sqrt(2)*np.cos(phi)*np.sin(lambdA/2)) / (np.sqrt(1+np.cos(phi)*np.cos(lambdA/2)))
	y_proj = (np.sqrt(2)*np.sin(phi)) / (np.sqrt(1+np.cos(phi)*np.cos(lambdA/2)))
	z_proj = np.sqrt(1-(0.25*x_proj)**2-(0.5*y_proj)**2)

	return [x_proj, y_proj, z_proj]

def RA_Proj(RA, DEC):
	#xy projection (scale axes using plotly window size)
	arc2Rad = np.pi/(3600*180)
	phi = DEC*arc2Rad
	lam = -1*RA*arc2Rad
	productVal = np.sqrt(2/(1+np.sin(phi)))
	xProjxy= productVal*np.cos(phi)*np.cos(lam)
	yProjxy = productVal*np.cos(phi)*np.sin(lam)

	#yz projection (scale axes using plotly window size)
	phi = DEC*arc2Rad
	lam = -1*RA*arc2Rad
	productVal = np.sqrt(2/(1+np.cos(phi)*np.cos(lam)))
	yProjyz= productVal*np.cos(phi)*np.sin(lam)
	zProjyz = productVal*np.sin(phi)

	#xz projection (scale axes using plotly window size)
	phi = DEC* arc2Rad
	lam = -1*RA*arc2Rad-np.pi/2
	productVal = np.sqrt(2/(1+np.cos(phi)*np.cos(lam)))
	xProjxz=productVal*np.cos(phi)*np.sin(lam)
	zProjxz = productVal*np.sin(phi)
	return [xProjxy, yProjxy, yProjyz, zProjyz, xProjxz, zProjxz]

def Galactic_Proj(lat, lon):
	 #galactic projections
			#xy
	deg2Rad = np.pi/180
	phi = lat *deg2Rad
	lam = lon*deg2Rad
	productVal = np.sqrt(2/(1+np.sin(phi)))
	xProjGxy= productVal*np.cos(phi)*np.cos(lam)
	yProjGxy = productVal*np.cos(phi)*np.sin(lam)
			#yz
	phi = lat*deg2Rad
	lam = lon*deg2Rad
	productVal = np.sqrt(2/(1+np.cos(phi)*np.cos(lam)))
	yProjGyz= productVal*np.cos(phi)*np.sin(lam)
	zProjGyz = productVal*np.sin(phi)
			#xz
	phi = lat* deg2Rad
	lam = lon*deg2Rad-np.pi/2
	productVal = np.sqrt(2/(1+np.cos(phi)*np.cos(lam)))
	xProjGxz=productVal*np.cos(phi)*np.sin(lam)
	zProjGxz = productVal*np.sin(phi)
	return [xProjGxy, yProjGxy, yProjGyz, zProjGyz, xProjGxz, zProjGxz]

def main():

	ngc2264 = pd.read_csv("NGC 2264 Errors 2.csv")
	ngc2264.head()


	ngc2264vel = ngc2264[["source_id", "ra", "dec", 'parallax']].rename(columns={"ra":"RA", "dec":"DE", "parallax":"Parallax"})

	ngc2264vel = ngc2264vel.dropna()
	
	# Option to see all the columns
	pd.set_option('display.max_columns', None)

	astro_df = ngc2264vel 

	# Removing rows with 0 values
	astro_df = astro_df[astro_df.RA != '']
	astro_df = astro_df[astro_df.DE != '']
	astro_df = astro_df[astro_df.Parallax != '']
	astro_df = astro_df[astro_df.Parallax != 0]
	astro_df['Distance'] = astro_df['Parallax'].apply(parse_parallax)
	astro_df = astro_df[astro_df.Distance != math.inf]

	# Parse the strings of RA and DE into arcseconds
	astro_df['RA_sc'] = astro_df.apply(lambda row: str_to_numbers(row['RA']), axis=1)
	astro_df['DE_sc'] = astro_df.apply(lambda row: str_to_numbers2(row['DE']), axis=1)
 

	astro_df['Galactic Longitude/Latitude'] = astro_df.apply(lambda row: galactic_longlat(row['RA_sc'], row['DE_sc']), axis=1)
	astro_df['Galactic Longitude'] = astro_df.apply(lambda row: row['Galactic Longitude/Latitude'][0], axis=1)
	astro_df['Galactic Latitude'] = astro_df.apply(lambda row: row['Galactic Longitude/Latitude'][1], axis=1)
	astro_df['Cartesian'] = astro_df.apply(lambda row: change_cartesian(row['RA_sc'], row['DE_sc'], row['Distance']), axis=1)
	astro_df['Cartesian X'] = astro_df.apply(lambda row: row['Cartesian'][0], axis=1)
	astro_df['Cartesian Y'] = astro_df.apply(lambda row: row['Cartesian'][1], axis=1)
	astro_df['Cartesian Z'] = astro_df.apply(lambda row: row['Cartesian'][2], axis=1)
	astro_df = astro_df.drop('Cartesian', axis=1)
	astro_df = astro_df.drop('Galactic Longitude/Latitude', axis=1)

	
	# 2-D Projection using RA-DEC
	#astro_df['RA-DEC Projection'] = astro_df.apply(lambda row: Coord_Proj(row['DE_sc'], row['RA_sc'], 'RA_DEC'), axis=1)
	#astro_df['RA-DEC ProjectionX'] = astro_df.apply(lambda row: row['RA-DEC Projection'][0], axis=1)
	#astro_df['RA-DEC ProjectionY'] = astro_df.apply(lambda row: row['RA-DEC Projection'][1], axis=1)
	#astro_df['RA-DEC ProjectionZ'] = astro_df.apply(lambda row: row['RA-DEC Projection'][2], axis=1)
	astro_df['RA-DEC Projection'] = astro_df.apply(lambda row: RA_Proj(row['RA_sc'], row['DE_sc']), axis=1)
	astro_df['RA-DEC XProjXY'] = astro_df.apply(lambda row: row['RA-DEC Projection'][0], axis=1)
	astro_df['RA-DEC YProjXY'] = astro_df.apply(lambda row: row['RA-DEC Projection'][1], axis=1)
	astro_df['RA-DEC YProjYZ'] = astro_df.apply(lambda row: row['RA-DEC Projection'][2], axis=1)
	astro_df['RA-DEC ZProjYZ'] = astro_df.apply(lambda row: row['RA-DEC Projection'][3], axis=1)
	astro_df['RA-DEC XProjXZ'] = astro_df.apply(lambda row: row['RA-DEC Projection'][4], axis=1)
	astro_df['RA-DEC ZProjXZ'] = astro_df.apply(lambda row: row['RA-DEC Projection'][5], axis=1)
	astro_df = astro_df.drop('RA-DEC Projection', axis=1)

	# 2-D Projection using Galactic Coordinates
	#astro_df['Galactic Projection'] = astro_df.apply(lambda row: Coord_Proj(row['Galactic Latitude'], row['Galactic Longitude'], 'Galactic'), axis=1)
	#astro_df['Galactic ProjectionX'] = astro_df.apply(lambda row: row['Galactic Projection'][0], axis=1)
	#astro_df['Galactic ProjectionY'] = astro_df.apply(lambda row: row['Galactic Projection'][1], axis=1)
	#astro_df['Galactic ProjectionZ'] = astro_df.apply(lambda row: row['Galactic Projection'][2], axis=1)
	astro_df['Galactic Projection'] = astro_df.apply(lambda row: Galactic_Proj(row['Galactic Latitude'], row['Galactic Longitude']), axis=1)
	astro_df['Galactic XProjXY'] = astro_df.apply(lambda row: row['Galactic Projection'][0], axis=1)
	astro_df['Galactic YProjXY'] = astro_df.apply(lambda row: row['Galactic Projection'][1], axis=1)
	astro_df['Galactic YProjYZ'] = astro_df.apply(lambda row: row['Galactic Projection'][2], axis=1)
	astro_df['Galactic ZProjYZ'] = astro_df.apply(lambda row: row['Galactic Projection'][3], axis=1)
	astro_df['Galactic XProjXZ'] = astro_df.apply(lambda row: row['Galactic Projection'][4], axis=1)
	astro_df['Galactic ZProjXZ'] = astro_df.apply(lambda row: row['Galactic Projection'][5], axis=1)
	astro_df = astro_df.drop('Galactic Projection', axis=1)

	print(astro_df)

	# Plot cartesian coordinates in 3-D Graph
	plt = px.scatter_3d(astro_df, x='Cartesian X', y='Cartesian Y', z='Cartesian Z', title='Position of Stars in NGC 2264')
	plt.show()

	#plts = []
	#plts = plot_2D(astro_df)
	#plts[0].show()
	#plts[1].show()
	#plts[2].show()
	#plts[3].show()
	#plts[4].show()
	#plts[5].show()


	#print(astro_df.head())




if __name__ == '__main__':
	main()