/*
 * particle_filter.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: Tiffany Huang
 */

#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

#include "particle_filter.h"

using namespace std;

void ParticleFilter::init(double x, double y, double theta, double std[]) {
	// TODO: Set the number of particles. Initialize all particles to first position (based on estimates of 
	//   x, y, theta and their uncertainties from GPS) and all weights to 1. 
	// Add random Gaussian noise to each particle.
	// NOTE: Consult particle_filter.h for more information about this method (and others in this file).
	
	num_particles=300;
	std::default_random_engine gen;
	std::normal_distribution<double> N_x(x,std[0]);
	std::normal_distribution<double> N_y(y,std[1]);
	std::normal_distribution<double> N_theta(theta,std[2]);
	

	for(int i=0;i<num_particles;i++)
	{
	double sample_x,sample_y,sample_theta;
	sample_x=N_x(gen);
	sample_y=N_y(gen);
	sample_theta=N_theta(gen);

	Particle particle;
	particle.id=i;
	particle.x=sample_x;
	particle.y=sample_y;
	particle.theta=sample_theta;
	particle.weight=1;
	
	particles.push_back(particle);
	weights.push_back(1); 
	}
	is_initialized=true;

}

void ParticleFilter::prediction(double delta_t, double std_pos[], double velocity, double yaw_rate) {
	// TODO: Add measurements to each particle and add random Gaussian noise.
	// NOTE: When adding noise you may find std::normal_distribution and std::default_random_engine useful.
	//  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
	//  http://www.cplusplus.com/reference/random/default_random_engine/

	std::default_random_engine gen;
	for(int i=0;i<num_particles;i++)
	{
	double new_x;
	double new_y;
	double new_theta;
		if(yaw_rate==0){
			new_x=particles[i].x+velocity*delta_t*cos(particles[i].theta);
			new_y=particles[i].y+velocity*delta_t*sin(particles[i].theta);
			new_theta=particles[i].theta;	
		}
		else{
			new_x=particles[i].x+(velocity/yaw_rate)*(sin(particles[i].theta+yaw_rate*delta_t)-sin(particles[i].theta));
			new_y=particles[i].y+(velocity/yaw_rate)*(cos(particles[i].theta)-cos(particles[i].theta+yaw_rate*delta_t));
			new_theta=particles[i].theta+yaw_rate*delta_t;
		}
	normal_distribution<double> N_x(new_x,std_pos[0]);
	normal_distribution<double> N_y(new_y,std_pos[1]);
	normal_distribution<double> N_theta(new_theta,std_pos[2]);
	
	particles[i].x=N_x(gen);
	particles[i].y=N_y(gen);
	particles[i].theta=N_theta(gen);

	}
	
}

void ParticleFilter::dataAssociation(std::vector<LandmarkObs> predicted, std::vector<LandmarkObs>& observations) {
	// TODO: Find the predicted measurement that is closest to each observed measurement and assign the 
	//   observed measurement to this particular landmark.
	// NOTE: this method will NOT be called by the grading code. But you will probably find it useful to 
	//   implement this method and use it as a helper during the updateWeights phase.
	
return;
}

std::vector<LandmarkObs> associateLandmarkPoints(std::vector<LandmarkObs> predicted, std::vector<LandmarkObs>& observations) {
	// TODO: Find the predicted measurement that is closest to each observed measurement and assign the 
	//   observed measurement to this particular landmark.


	std::vector<LandmarkObs> associated_lndmrks;
	LandmarkObs closest_lndmrk;

	for(int i=0;i<observations.size();i++)
	{
		double x_obs =observations[i].x;
		double y_obs =observations[i].y;	
		double min_dist=1E15; // some distance

		for(int j=0;j<predicted.size();j++)
		{
			double x_pred=predicted[j].x;
			double y_pred=predicted[j].y;
			double near_dist=dist(x_obs,y_obs,x_pred,y_pred);
			if(near_dist<min_dist)
			{
				min_dist=near_dist;
				closest_lndmrk=predicted[j];			
			}
		}
		associated_lndmrks.push_back(closest_lndmrk);
	}	
	return associated_lndmrks;
}


double calc_mulpdf(double sigma_x,double sigma_y,double mu_x,double mu_y,double x,double y){
// calc the multi variate PDF	

	double exponent=((x-mu_x)*(x-mu_x))/(2*(sigma_x)*(sigma_x))+((y-mu_y)*(y-mu_y))/(2*(sigma_y)*(sigma_y));

	double pdf=(1.0/(2*M_PI*sigma_x*sigma_y));

     	double mul_gauss=pdf*exp(-exponent);

    	return mul_gauss;
}


void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
		const std::vector<LandmarkObs> &observations, const Map &map_landmarks) {
	// TODO: Update the weights of each particle using a mult-variate Gaussian distribution. You can read
	//   more about this distribution here: https://en.wikipedia.org/wiki/Multivariate_normal_distribution
	// NOTE: The observations are given in the VEHICLE'S coordinate system. Your particles are located
	//   according to the MAP'S coordinate system. You will need to transform between the two systems.
	//   Keep in mind that this transformation requires both rotation AND translation (but no scaling).
	//   The following is a good resource for the theory:
	//   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
	//   and the following is a good resource for the actual equation to implement (look at equation 
	//   3.33
	//   http://planning.cs.uiuc.edu/node99.html
	
	double sigma_x=std_landmark[0];
	double sigma_y=std_landmark[1];
	
	for(int i=0;i<particles.size();i++)
	{
	  Particle par=particles[i];
	  // crteate transformations vector array	
	  std::vector<LandmarkObs> transObservations;
	  for(int j=0;j<observations.size();j++)
	  {
	    	LandmarkObs transObs;
		transObs.x=par.x+observations[j].x*cos(par.theta)-sin(par.theta)*observations[j].y;
		transObs.y=par.y+observations[j].x*sin(par.theta)+cos(par.theta)*observations[j].y;
		transObs.id=observations[j].id;
		transObservations.push_back(transObs);
	  }
	
	  std::vector<LandmarkObs> predicted;
	  for(int k=0;k<map_landmarks.landmark_list.size();k++)
	  {
		double distance=dist(par.x,par.y,map_landmarks.landmark_list[k].x_f,map_landmarks.landmark_list[k].y_f);
		if(distance<sensor_range)
		{
			LandmarkObs landmrk;
			landmrk.x=map_landmarks.landmark_list[k].x_f;
			landmrk.y=map_landmarks.landmark_list[k].y_f;
			landmrk.id=map_landmarks.landmark_list[k].id_i;			
			predicted.push_back(landmrk);
		}
	  }	

	 std::vector<LandmarkObs> associatedObs=associateLandmarkPoints(predicted,transObservations);
	
	 double probability=1;
	 for(int a=0;a<associatedObs.size();++a)
	 {
	  probability*=calc_mulpdf(sigma_x,sigma_y,associatedObs.at(a).x,associatedObs.at(a).y,transObservations.at(a).x,transObservations.at(a).y);
	  
	 }
	par.weight=probability;
	weights[i]=probability;
	}

}

void ParticleFilter::resample() {
	// TODO: Resample particles with replacement with probability proportional to their weight. 
	// NOTE: You may find std::discrete_distribution helpful here.
	//   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution

	std::default_random_engine gen;
	discrete_distribution<int> distribution(weights.begin(),weights.end());

	vector<Particle> resample_particles;
	for(int i=0;i<num_particles;i++)
	{
		resample_particles.push_back(particles[distribution(gen)]);
	}
	particles=resample_particles;
}

Particle ParticleFilter::SetAssociations(Particle& particle, const std::vector<int>& associations, 
                                     const std::vector<double>& sense_x, const std::vector<double>& sense_y)
{
    //particle: the particle to assign each listed association, and association's (x,y) world coordinates mapping to
    // associations: The landmark id that goes along with each listed association
    // sense_x: the associations x mapping already converted to world coordinates
    // sense_y: the associations y mapping already converted to world coordinates

    particle.associations= associations;
    particle.sense_x = sense_x;
    particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best)
{
	vector<int> v = best.associations;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<int>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseX(Particle best)
{
	vector<double> v = best.sense_x;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseY(Particle best)
{
	vector<double> v = best.sense_y;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
