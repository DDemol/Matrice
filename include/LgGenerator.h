#pragma once

/*!
 * \file LgGenerator.h
 * \brief File containing the Generator class, the mother class for implementing any Node which only add data.
 * \author Bremard Nicolas
 * \version 0.2
 * \date 02 july 2014
 */

#ifndef LGGENERATOR_H_
#define LGGENERATOR_H_

/*!
* \brief Type of Node for Generators
*/
#define LG_NODE_GENERATOR "LG_NODE_GENERATOR"

#include <map>
#include <string>
#include <set>

#include "LgNode.h"

using namespace std;

/*! 
 * \namespace lg
 * \brief Namespace containing everything for managing libGina Environment, Node and Groups.
*/
namespace lg
{

	/*!
	 * \class Generator
	 * \brief Mother class for creating Node which purely add data.
	 * A Generator is a Node which injects new data in the loop. Those data can come from a device, network, ...
	 * Any Environment must, at least, have one Generator registered in.
	*/
	class Generator : public Node
	{
	public:

		/*!
		 * \brief Constructor for creating a new Generator
		 * Create a new Generator with a name, the type of the Node is LG_NODE_GENERATOR
		 * \param[in] name : a name for the instance of this Node
		 */
		Generator(string name);

		/*!
		 * \brief Constructor by copy
		 * \param[in] copy : another instance of this Node from which copy the details (name, id...)
		 */
		Generator(const Generator &copy);

		/*!
		 * \brief Destructor
		 */
		virtual ~Generator(void);

		/*!
		 * \brief Update the Generator with current data 
		 * Function calling the virtual generate function and implementing the Data Copy option
		 * For avoiding side effect, Groups are not passed directly in order to be modified.
		 * They can be passed by copy, then after additional data have been injected, these data are copied in the Groups.
		 * \param[in] groups3D : Groups of HOrientedPoint3D
		 * \param[in] groups2D : Groups of HOrientedPoint2D
		 * \param[in] groups1D : Groups of HOrientedPoint1D
		 * \param[in] groupsSwitch : Groups of HSwitch
		 * \return true if success
		 */
		bool update(map<string,Group3D*>& groups3D, map<string,Group2D*>& groups2D, map<string,Group1D*>& groups1D, map<string,GroupSwitch*>& groupsSwitch);

		/*!
		 * \brief Generate data then inject it in the specific Groups
		 * Virtual function that must be implemented by any Generator. It is the main function of the Generator.
		 * \param[in] groups3D : Groups of HOrientedPoint3D
		 * \param[in] groups2D : Groups of HOrientedPoint2D
		 * \param[in] groups1D : Groups of HOrientedPoint1D
		 * \param[in] groupsSwitch : Groups of HSwitch
		 * \return true if success
		 */
		virtual bool generate(map<string,Group3D*>& groups3D, map<string,Group2D*>& groups2D, map<string,Group1D*>& groups1D, map<string,GroupSwitch*>& groupsSwitch) = 0;

		/*!
		 * \brief Return an empty set of string because a Generator doesn't have any data in input
		 * This function is used by the Environment to check if the registered Node are compatible (ie Node require data types which is present).
		 * \return An empty set
		 */
		set<string> consume() const {return set<string>();} 

		/*!
		 * \brief Return an empty set of string because a Generator doesn't have any data in input
		 * This function is used by the Environment to check if the registered Node are compatible (ie Node require data types which is present).
		 * \return An empty set
		 */
		set<string> need() const {return set<string>();} 

		/*!
		 * \brief Allow to generate only specific type
		 * If the specified type is not generated by the Generator, it doesn't do anything.
		 * If not used, all type are generated
		 */
		void onlyGenerate(string typeToGenerate) {_generatedType[typeToGenerate] = true;} 

		/*!
		 * \brief Check if a specific type is generated by this Generator
		 * \return true if this Generator generates the given type
		 */
		bool isGenerated(string typeToGenerate) const {return ((_generatedType.size() == 0)||(_generatedType.at(typeToGenerate)));} 

	protected:
		map<string,Group3D*> _local3Dgroups;
		map<string,Group2D*> _local2Dgroups;
		map<string,Group1D*> _local1Dgroups;
		map<string,GroupSwitch*> _localSwitchgroups;

		map<string,bool> _generatedType;

	};

}

#endif /* LGGENERATOR_H_ */