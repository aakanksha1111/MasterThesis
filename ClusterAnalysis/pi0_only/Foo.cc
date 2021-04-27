// -*- C++ -*-
//
// This is the implementation of the non-inlined, non-templated member
// functions of the Foo class.
//

#include "Foo.h"
#include "ThePEG/Interface/ClassDocumentation.h"
#include "ThePEG/EventRecord/Particle.h"
#include "ThePEG/Repository/UseRandom.h"
#include "ThePEG/Repository/EventGenerator.h"
#include "ThePEG/Utilities/DescribeClass.h"



using namespace MyName;

Foo::Foo() {}

Foo::~Foo() {}



#ifndef LWH_AIAnalysisFactory_H
#ifndef LWH 
#define LWH ThePEGLWH
#endif
#include "ThePEG/Analysis/LWH/AnalysisFactory.h"
#endif

void Foo::analyze(tEventPtr event, long ieve, int loop, int state)
{
  Lorentz5Momentum mtm;
  double a,b,c,d,e;
  std::fstream f;
  // f.open("sample1.txt",ios::out | ios::in | ios::trunc); // opens file by clrearin it out first
  // f.open("sample1.txt",ios::out | ios::in | ios::app );
 
  // f<< " This is sample line of text for event number : " << event->number() << endl;
 
  int flag=0;
  int reqmainclustercount=0;
  int kids_count[50];
  int mainclustercount=0;
  int clusterCount=0,loopcount=0;
  set<tcPPtr> particles;
  // event->selectFinalState(inserter(particles));
  event->select(inserter(particles), ThePEG::AllSelector());
  generator() -> log() << " Sample Text in Log File \n" << endl;
  
  std::cout << "-------------------------------------------------------------------------------------- " << endl;
  std::cout << " Event Number : " << event->number() << "\t" <<  endl;
  
  // std::cout << " Heavy Clusters Decay into smaller clusters, Clusters at the leaf of such cluster tress are considered for analysis \n\n " << endl; 
  for(set<tcPPtr>::const_iterator it = particles.begin(); it != particles.end();it++)  // Start a loop traversing through all of the particles that are produced in event
  { 
    
    if((**it).id()==81)   // Enter If it is a Cluster 
      {
	//	std::cout << "-------------------------------------------------------------------------------------- " << endl;
	tParticleSet bros;        // Ir-relevant here
	bros= (**it).siblings(); // Ir - relevant here

	ParticleVector kids;
	kids = (**it).children();  // Store all consecutive children of the cluster
	
	
        if(isMainCluster((**it).parents()))  // Check if It is a Primary=main cluster, If Yes, then only enter
	  {
	      // std::cout << " Cluster with Order number : " << (**it).number() << " Is a Main Cluster \n " << endl;
	    
	     kids_count[mainclustercount]=0;  // array to store number of consecutive children of different main clusters
	     flag=0;                         
	     for(ParticleVector::const_iterator it6 = kids.begin();it6 != kids.end();it6++)
	       { 
           
		   
		 // std::cout << " Main Cluster with Order Number : " << (**it).number() << " is origin  of following final state Particles \n " << endl;
                 if( (**it6).id() == 111)  // If child is a particle 
		   {
		     kids_count[mainclustercount] += 1; // Increment count of kids
                   }
		 else
                   {
                     flag++;  // increment number of cluster the primary cluster decays into
		   }
		  
		 // std::cout << " Total number of Final state particles produced from it : " << kids_count[mainclustercount] <<  endl;
	       } 
	     if(kids_count[mainclustercount]==2 && flag==0  ) // We will focus only on Primary clusters which decay into only 2 particles as of now
	       {
                
		 f.open("new.csv",ios::out | ios::in | ios::app ); // Open file to store data Of cluster and 2 particles it decays into
                    
		 mtm = (*it)->momentum(); 
	         a= mtm.m()/MeV;   // Mass in MeV
	         b=mtm.x()/MeV;     // px in MeV
	         c= mtm.y()/MeV;    // py in MeV
	         d=mtm.z()/MeV;     // pz in MeV
	         e=mtm.t()/MeV;     // E in MeV

		 // Store 8 variables for each cluster and particle in following order

		 // Event number, PDG ID, E, px ,py, pz , Mass , (px)^2 + (py)^2 + (pz)^2 + (Mass)^2 - (E)^2
                     
	         f << event->number()<<"," << (**it).id()<<","<<e<<","<<b<<","<<c<<","<<d<<","<<a<<","<<(b*b+c*c+d*d+a*a-e*e)<<endl; 
                 for(ParticleVector::const_iterator it7 = kids.begin();it7 != kids.end();it7++)  // go through children of cluster to store their data
	         { 
                 
		    mtm = (*it7)->momentum();
	            a= mtm.m()/MeV;
	            b=mtm.x()/MeV;
	            c=mtm.y()/MeV;
	            d=mtm.z()/MeV;
	            e=mtm.t()/MeV;
                     
	             f << event->number()<<","<< (**it7).id()<<","<<e<<","<<b<<","<<c<<","<<d<<","<<a<<","<<(b*b+c*c+d*d+a*a-e*e)<<endl; 
	         	 // std::cout << " Main Cluster with Order Number : " << (**it).number() << " is origin  of following final state Particles \n " << endl;
                
		  
		  // std::cout << " Total number of Final state particles produced from it : " << kids_count[mainclustercount] <<  endl;
	          } 
  
		 f.close(); // close the file
		 reqmainclustercount++;  // Increment number of Required main clusters
	       }
	        
	     
	     mainclustercount++;  // Increment the number of Main clusters
	  }
	
	// ------------------------------------------------------------------------------------------------------- Ir- relevant for our use-------------------------------------
          	// 	for(set<tPPtr>::const_iterator it2 = bros.begin();it2 != bros.end();it2++) // for bros
        	/*	for(ParticleVector::const_iterator it2 = kids.begin();it2 != kids.end();it2++) // leaf cluster decay details
	      {
               if( (**it2).id() != 81 )
	         {
	         	std::cout << " Cluster with Order Number : " << (**it).number() << " is origin  of following final state Particles \n " << endl;
		    kids_count[clusterCount] = calc(kids);
		    std::cout << " Total number of Final state particles produced from it : " << kids_count[clusterCount] <<  endl;											         
	   	    
	          }
	         // std::cout << " ID of kid : " << (**it2).id() << endl;
	   
	      } */
	      //	std::cout << "Number of Kids of " << (clusterCount+1) <<" Cluster : " << kids_count[clusterCount] << endl;
	// --------------------------------------------------------------------------------------------------------------------------------------------------------

	clusterCount++; // Increment number of Clusters
	
	}      
     // std::cout << "\n Particle " << (loopcount+1) << " :\n\tOrder Number : "<< (**it).number() << " ID : "<< (**it).id() << " colour info : " << (**it).hasColourInfo() << endl;
     // hasColourInfo() gives zero unless its a pure quark, think so
     //  std::cout << " ID of " << (loopcount+1) << " Particle : " << (**it).id() <<endl;
     //  std::cout << " Order number of " << (loopcount+1) << " Particle : " << (**it).number() <<endl;
    loopcount++;
  }
   std::cout << "Number of Main Clusters Total :\t " << mainclustercount << std::endl;
   std::cout << "Number of Main Clusters Decaying \n Directly into 2 particles only Total :\t " << reqmainclustercount << std::endl;
   std::cout << "Number of Clusters Total :\t " << clusterCount << std::endl << endl;
   // std::cout << "Number of Particles total :" << loopcount << std::endl;
   // std::cout << "Event number :\t" << event->number() <<endl;
  AnalysisHandler::analyze(event, ieve, loop, state);
  // Rotate to CMS, extract final state particles and call analyze(particles).
}

bool Foo::isMainCluster(tParticleVector mothers)    // Returns true if a cluster is Primamry, else false
{

  for(tParticleVector::const_iterator it4 = mothers.begin();it4 != mothers.end();it4++)
    {
      if( (**it4).id() == 81)
	{
	  return false;
	}
      
    } 
  return true;
}


int Foo::calc(ParticleVector daughters)  // Prints information of Final state particles from a given set of daughters, Ir- relevent here 
{
  int np=0;
  Lorentz5Momentum mmtm;
  double a,b,c,d,e;
  for(ParticleVector::const_iterator it3 = daughters.begin();it3 != daughters.end();it3++)
     {
       if((**it3).decayed())
	 {
	   np = np + calc((**it3).children());
	 }
       else
	 {
           mmtm = (*it3)->momentum();
	   a= mmtm.m()/MeV;
	   b=mmtm.x()/MeV;
	   c= mmtm.y()/MeV;
	   d=mmtm.z()/MeV;
	   e=mmtm.t()/MeV;
	   std::cout <<  " Particle : " <<  (**it3).PDGName() << "\tID : " <<  (**it3).id() <<   endl;
	   std::cout <<  " Lorentz 5 Momentum " << a << " " << b << " " << c << " " << d << " " << e << " " << endl;
	   std::cout <<  " Lorentz 5 Momentum " << mmtm.m()/MeV << " " <<  mmtm.x()/MeV << " "  << mmtm.y()/MeV <<" " << mmtm.z()/MeV  << " " << mmtm.t()/MeV  << endl;
	   std::cout << " P^2 + m^2 -E^2 : " << (a*a + b*b + c*c + d*d - e*e); 
   std::cout << "\n" << endl;
         np++;
	 }
   	       	 
     }
  return np;
}


LorentzRotation Foo::transform(tcEventPtr event) const {
  return LorentzRotation();
  // Return the Rotation to the frame in which you want to perform the analysis.
}

void Foo::analyze(const tPVector & particles, double weight) {
  
  AnalysisHandler::analyze(particles);
  // Calls analyze() for each particle.
  
  std::cout << "Test2 " << std::endl; 	

}

void Foo::analyze(tPPtr, double weight) {}

void Foo::dofinish() {
  AnalysisHandler::dofinish();
  // *** ATTENTION *** Normalize and post-process histograms here.
}

void Foo::doinitrun() {
  AnalysisHandler::doinitrun();
  // *** ATTENTION *** histogramFactory().registerClient(this); // Initialize histograms.
  // *** ATTENTION *** histogramFactory().mkdirs("/SomeDir"); // Put histograms in specal directory.
}


IBPtr Foo::clone() const {
  return new_ptr(*this);
}

IBPtr Foo::fullclone() const {
  return new_ptr(*this);
}


// If needed, insert default implementations of virtual function defined
// in the InterfacedBase class here (using ThePEG-interfaced-impl in Emacs).



// *** Attention *** The following static variable is needed for the type
// description system in ThePEG. Please check that the template arguments
// are correct (the class and its base class), and that the constructor
// arguments are correct (the class name and the name of the dynamically
// loadable library where the class implementation can be found).
DescribeNoPIOClass<Foo,AnalysisHandler>
  describeMyNameFoo("MyName::Foo", "Foo.so");

void Foo::Init() {

  static ClassDocumentation<Foo> documentation
    ("There is no documentation for the Foo class");

}
