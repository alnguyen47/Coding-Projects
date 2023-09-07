
# Import libraries
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import PyAstronomy as pyAst
import math


# Import csv file
ngc2264 = pd.read_csv("NGC 2264.csv")
ngc2264.head()


ngc2264vel = ngc2264[["source_id", "ra", "dec", 'parallax', 'pmra', 'pmdec', 'radial_velocity']]

ngc2264vel = ngc2264vel.dropna()
#ngc2264vel
ngc2264vel.shape

# Determine the mean and standard deviation, max, min, range
parallax_mean = ngc2264vel['parallax'].mean()
parallax_std = ngc2264vel['parallax'].std()
parallax_min = ngc2264vel['parallax'].min()
parallax_max = ngc2264vel['parallax'].max()
parallax_range = parallax_max - parallax_min

pmra_mean = ngc2264vel['pmra'].mean()
pmra_std = ngc2264vel['pmra'].std()
pmra_min = ngc2264vel['pmra'].min()
pmra_max = ngc2264vel['pmra'].max()
pmra_range = pmra_max - pmra_min 

pmdec_mean = ngc2264vel['pmdec'].mean()
pmdec_std = ngc2264vel['pmdec'].std()
pmdec_min = ngc2264vel['pmdec'].min()
pmdec_max = ngc2264vel['pmdec'].max()
pmdec_range = pmdec_max - pmdec_min

radial_vel_mean = ngc2264vel['radial_velocity'].mean()
radial_vel_std = ngc2264vel['radial_velocity'].std()
radial_vel_min = ngc2264vel['radial_velocity'].min()
radial_vel_max = ngc2264vel['radial_velocity'].max()
radial_vel_range = radial_vel_max - radial_vel_min

stats_ngc2264 = pd.DataFrame({'parallax': [parallax_mean, parallax_std, parallax_min, parallax_max, parallax_range], 
                              'pmra': [pmra_mean, pmra_std, pmra_min, pmra_max, pmra_range],
                              'pmdec': [pmdec_mean, pmdec_std, pmdec_min, pmdec_max, pmdec_range],
                              'radial_velocity': [radial_vel_mean, radial_vel_std, radial_vel_min, radial_vel_max, radial_vel_range]}).rename(index={0: 'mean', 1: 'std', 2: 'min', 3: 'max', 4: 'range'})
stats_ngc2264


# Calculate z-scores
ngc2264vel['parallax_z_score'] = ngc2264vel.apply(lambda x: (x['parallax']-parallax_mean)/parallax_std, axis=1)
ngc2264vel['pmra_z_score'] = ngc2264vel.apply(lambda x: (x['pmra']-pmra_mean)/pmra_std, axis=1)
ngc2264vel['pmdec_z_score'] = ngc2264vel.apply(lambda x: (x['pmdec']-pmdec_mean)/pmdec_std, axis=1)
ngc2264vel['radial_vel_z_score'] = ngc2264vel.apply(lambda x: (x['radial_velocity']-radial_vel_mean)/radial_vel_std, axis=1)

ngc2264vel

# Determine outliers(z-scores > 2)
ngc2264vel['parallax_outlier'] = ngc2264vel.apply(lambda x: True if abs(x['parallax_z_score']) > 2 else False, axis=1)
ngc2264vel['pmra_z_score'] = ngc2264vel.apply(lambda x: True if abs(x['pmra_z_score']) > 2 else False, axis=1)
ngc2264vel['pmdec_z_score'] = ngc2264vel.apply(lambda x: True if abs(x['pmdec_z_score']) > 2 else False, axis=1)
ngc2264vel['radial_vel_z_score'] = ngc2264vel.apply(lambda x: True if abs(x['radial_vel_z_score']) > 2 else False, axis=1)

ngc2264vel


