#ifndef PTDESIGNER_H
#define PTDESIGNER_H

//**********************************************************************

/** @file
 * Header file providing procedural textures designer C++ interface. It
 * allows the programmer to create textures by describing it as a graph
 * of nodes (blocks), of which each represents basic operations for
 * creating textures.
 *
 * @author Miloslav Ciz
 */

/*
 * Copyright 2014 Miloslav Číž
 *
 * This file is part of PT Designer.
 *
 * PT Designer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PT Designer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with these files. If not, see <http://www.gnu.org/licenses/>.
 */
 
//**********************************************************************

extern "C"
{
#include "colorbuffer.h"
#include "colortransition.h"
#include "matrix.h"
#include "grammar.h"
#include "proctextures.h"
#include "general.h"
}

#include <iostream>
#include <vector>
#include <string>
#include <locale.h>
#include "rapidxml.hpp"

#define PT_LIB_VERSION    "1.0"  /// library version
#define MAX_INPUT_BLOCKS  5  /// maximum number of input blocks
#define MAX_SUPERSAMPLING 6  /// maximum supersampling level

using namespace std;
using namespace rapidxml;

namespace pt_design
{

// forward declarations:

class c_texture_graph;
class c_block;

  /**
   * Possible parameter data types.
   */

typedef enum
  {
    PARAMETER_INT,
    PARAMETER_DOUBLE,
    PARAMETER_BOOL,
    PARAMETER_STRING
  } t_parameter_type;

typedef struct
  {
    t_parameter_type type;
    string name;

    union
      {
        int int_value;
        double double_value;
        bool bool_value;
      };

    string string_value;
  } t_parameter;

//----------------------------------------------------------------------

  /**
   * Holds and manages a set of parameters. The parameters can be either
   * of int, double, bool or string data type.
   */

class c_parameters

  {
    protected:
      vector<t_parameter> *parameters;   /// holds the parameters
      c_block *block;                    /** block that this object
                                             belongs to, it is
                                             invalidated everythime
                                             parameters change */

      bool locked;                       /** whether the parameter
                                             creation is locked */
      int index_by_name(string name);

        /**<
         * Returns an index of parameter with given name.
         *
         * @param name name of the parameter
         *
         * @return index of the parameter or -1 if the parameter with
         *         the name doesn't exist
         */

      void changed();

        /**<
         * This should be called when a change in parameters occur.
         * Invalidation of the owner block will be performed.
         */

    public:
      c_parameters(c_block *owner);

        /**<
         * Class constructor, creates a new object.
         *
         * @param owner block that this object belong to, it will be
         *        invalidated everytime a change in parameters occur
         */

      ~c_parameters();

        /**<
         * Class destructor.
         */

      unsigned int number_of_parameters();

        /**<
         * Returns number of parameters.
         *
         * @return number of parameters
         */

      string get_value_string(unsigned int index);

        /**<
         * Returns a string representation of value of parameter at
         * given index.
         *
         * @param index parameter index
         *
         * @return string representing the parameter value or an empty
         *         string if the index is outside range
         */

      void lock();

        /**<
         * Lock creation of new parameters. Once this function has been
         * called, it is impossible to add new parameters to this
         * object.
         */

      bool is_locked();

        /**<
         * Checks if the object is locked for new parameter creation.
         *
         * @return true if the object is locked (new parameters can't be
         *         created), otherwise false
         */

      t_parameter_type get_type(string name);

        /**<
         * Gets data type of given parameter.
         *
         * @param name name of the parameter, it must exist
         *
         * @return parameter data type
         */

      t_parameter_type get_type(unsigned int index);

        /**<
         * Gets data type of given parameter.
         *
         * @param index index of the parameter, it must exist
         *
         * @return parameter data type
         */

      string get_name(unsigned int index);

        /**<
         * Gets name of the patameter with given index.
         *
         * @param index parameter index
         *
         * @return parameter name
         */

      bool add_parameter(string name, t_parameter_type type);

        /**<
         * Adds a new parameter to the object.
         *
         * @param name case-sensitive parameter name, must be unique
         *        within the object
         * @param type parameter data type
         *
         * @return true if the parameter was added, false otherwise
         */

      bool set_int_value(string parameter_name, int value);

        /**<
         * Sets an integer value of the parameter.
         *
         * @param parameter_name parameter name
         * @param value value to be set
         *
         * @return true if the value was set, false otherwise
         */

      bool set_double_value(string parameter_name, double value);

        /**<
         * Sets a double value of the parameter.
         *
         * @param parameter_name parameter name
         * @param value value to be set
         *
         * @return true if the value was set, false otherwise
         */

      bool set_bool_value(string parameter_name, bool value);

        /**<
         * Sets a bool value of the parameter.
         *
         * @param parameter_name parameter name
         * @param value value to be set
         *
         * @return true if the value was set, false otherwise
         */

      bool set_string_value(string parameter_name, char *value);

        /**<
         * Sets a string value of the parameter.
         *
         * @param parameter_name parameter name
         * @param value value to be set
         *
         * @return true if the value was set, false otherwise
         */

      bool set_string_value(string parameter_name, string value);

        /**<
         * Sets a string value of the parameter.
         *
         * @param parameter_name parameter name
         * @param value value to be set
         *
         * @return true if the value was set, false otherwise
         */

      int get_int_value(string parameter_name);

        /**<
         * Gets an integer value of the parameter. If the parameter is
         * not of integer data type, undefined value is returned.
         *
         * @param parameter_name parameter name
         *
         * @return parameter value
         */

      double get_double_value(string parameter_name);

        /**<
         * Gets a double value of the parameter. If the parameter is
         * not of double data type, undefined value is returned.
         *
         * @param parameter_name parameter name
         *
         * @return parameter value
         */

      bool get_bool_value(string parameter_name);

       /**<
         * Gets a bool value of the parameter. If the parameter is
         * not of bool data type, undefined value is returned.
         *
         * @param parameter_name parameter name
         *
         * @return parameter value
         */

      string get_string_value(string parameter_name);

        /**<
         * Gets a string value of the parameter. If the parameter is
         * not of string data type, undefined value is returned.
         *
         * @param parameter_name parameter name
         *
         * @return parameter value
         */

      int get_int_value(unsigned int index);

        /**<
         * Gets an integer value of the parameter. If the parameter is
         * not of integer data type, undefined value is returned.
         *
         * @param inde parameter index
         *
         * @return parameter value
         */

      double get_double_value(unsigned int index);

        /**<
         * Gets a double value of the parameter. If the parameter is
         * not of double data type, undefined value is returned.
         *
         * @param inde parameter index
         *
         * @return parameter value
         */

      bool get_bool_value(unsigned int index);

       /**<
         * Gets a bool value of the parameter. If the parameter is
         * not of bool data type, undefined value is returned.
         *
         * @param inde parameter index
         *
         * @return parameter value
         */

      string get_string_value(unsigned int index);

        /**<
         * Gets a string value of the parameter. If the parameter is
         * not of string data type, undefined value is returned.
         *
         * @param inde parameter index
         *
         * @return parameter value
         */
  };

//----------------------------------------------------------------------

 /**
  * Represents a general texture designer block, which is a node of
  * texture describing graph. To create a block instance use the static
  * factory method get_block_instance.
  */

class c_block

  {
    protected:
      unsigned int id;          /// block id (unique within the graph)
      bool is_end_block;        /// whether the block is terminal
      bool valid;               /// whether the block is currently valid
      bool error;               /// whether there was any error
      c_block *(input_blocks[MAX_INPUT_BLOCKS]); /// input blocks
      unsigned int inputs;      /// current number of inputs
      unsigned int max_inputs;  /// maximum number of inputs
      unsigned int min_inputs;  /// minimum number of inputs
      c_texture_graph *graph;   /// graph that the block belongs to
      string name;              /// name of the block
      bool initialised;         /// whether the block is initialised
      c_parameters *parameters; /// block parameters
      bool uses_global_seed;    /** whether global or custom random seed
                                    is used */
      int custom_seed;          /// custom seed value

      void set_error();

        /**<
         * Sets the error flag and invalidates the block.
         */

      bool is_graphic_input(unsigned int number);

        /**<
         * Checks whether there is a graphic block at given input block
         * of this block.
         *
         * @param number number of input slot
         *
         * @return true if there is a graphic block at given input slot,
         *         false otherwise
         */

      virtual void set_default();

        /**<
         * Sets block specific parameters to default values.
         */

      virtual bool execute();

        /**<
         * Computes the block output and executes all other asociated
         * actions. The function presumes the input blocks are
         * already computed and valid.
         *
         * @return true if everything was done OK, false otherwise
         *         (an error occured)
         */

    public:

      c_block();

        /**<
         * Class constructor, initialises a new block. The block can't
         * be used before it's added to a texture graph.
         */

      bool is_terminal();

        /**<
         * Checks if the block is terminal, i.e. cannot serve as a
         * input for another block.
         *
         * @return true if the block is terminal, false otherwise
         */

      static c_block *get_block_instance(string block_name);

        /**
         * Creates an instance of concrete c_block subclass depending
         * on provided string name of the block.
         *
         * @param block_name identifies which instance subclass instance
         *        should be created
         *
         * @return concrete newly allocated c_block subclass or NULL if
         *         the string does not identify any subclass
         */

      virtual bool load_parameters(string parameters);

        /**<
         * Sets the block parameters specified by string.
         *
         * @param parameters parameter describing string
         *
         * @return true if the parameters were set succesfully, false
         *         otherwise
         */

      unsigned int get_min_inputs();
      
        /**<
         * Returns a minimum number of inputs for this block.
         * 
         * @return minimum number of input blocks for this block
         */
         
      unsigned int get_max_inputs();
      
        /**<
         * Returns a maximum number of inputs for this block.
         * 
         * @return maximum number of input blocks for this block
         */

      c_parameters *get_parameters();

        /**<
         * Returns a pointer to the block parameter-handling object.
         *
         * @return block parameters
         */

      void set_texture_graph(c_texture_graph *graph);

        /**<
         * Sets the block to belong to specified texture graph.
         *
         * @param graph texture graph which this block should belong to
         */

      int get_random_seed();

        /**<
         * Returns random seed value used by this block (either the
         * global value or custom set value). This should be used by
         * the block to get random seed value during computations.
         *
         * @return random seed value
         */

      c_block *get_input(unsigned int index);

        /**<
         * Gets the block input at specified input slot.
         *
         * @param index input slot index
         *
         * @return pointer to input block at given input slot of this
         *         block
         */

      bool has_ancestor(c_block *block);

        /**<
         * Checks if the block has another block as its ancestor in any
         * depth.
         *
         * @return true if the block block is this block's ancestor
         *         (even indirect), otherwise false
         */

      bool is_using_global_seed();

        /**<
         * Checks if the block uses global or custom random seed value.
         *
         * @return true if global seed (set in the texture graph) is
         *         used, false if custom seed is used
         */

      void use_global_seed();

        /**<
         * Sets the block so that it will use the global seed set in the
         * texture graph instead of custom seed (global seed is used by
         * default).
         */

      void use_custom_seed(int value);

        /**<
         * Sets the block so that it will use custom random seed value
         * instead of global seed.
         *
         * @param value custom random seed value to be set
         */

      virtual void adjust();

        /**<
         * Adjusts the block parameters to current graph parameters
         * (i.e. resolution and so on)
         */

      string get_name();

        /**<
         * Returns text name of the block.
         *
         * @return name of the block
         */

      void set_id(unsigned int new_id);

        /**<
         * Sets the id for the block.
         *
         * @param id new id to be set
         */

      unsigned int get_id();

        /**<
         * Returns the block id.
         *
         * @return the block id
         */

      virtual bool has_image();

        /**<
         * Checks if the given block can provide a color buffer with
         * image.
         *
         * @return true if this block has a color buffer, false
         *         otherwise
         */

      bool connect(c_block *input_block, unsigned int slot_number);

        /**<
         * Connects another block to this block's input.
         *
         * @param input_block block to be connected to this block's
         *        input slot
         * @param slot_number number of this block's input slot to
         *        connect the input block to
         *
         * @return true if the block was connected, otherwise false
         *         (the blocks cannot be connected if they make a cycle
         *         in the graph)
         */

      void disconnect(unsigned int slot_number);

        /**<
         * Disconnects an input block on given input slot.
         *
         * @param slot_number slot to be disconnected
         */

      void invalidate();

        /**<
         * Invalidates the block so that it's output will be recomputed
         * next time it's needed, even if it wasn't necessarry.
         */

      bool is_valid();

        /**<
         * Checks if the block output is currently valid.
         *
         * @return true if the output is valid, false otherwise
         */

      bool is_error();

        /**<
         * Checks if there was any error when generating the block
         * output.
         *
         * @return true if there was an error, false otherwise
         */

      bool is_user_of(c_block *block);

        /**<
         * Check if this block uses another block as an input.
         *
         * @param block block which this block is checked to be using
         *
         * @return true if this block uses the block block as an input,
         *         false otherwise
         */

      virtual ~c_block();

        /**<
         * Class destructor, destroys the block and frees all it's
         * memory.
         */

      virtual void set_default_parameters();

        /**<
         * Sets the block parameters to default values
         */

      bool compute(bool force);

        /**<
         * Computes the block output. If the block has any input blocks,
         * it will check whether they are valid and will recursively
         * recompute them if needed.
         *
         * @param force if true, the block will recompute it's output
         *        and all input blocks always, even if it's not
         *        necesarry, otherwise recomputing will only be done
         *        if needed
         *
         * @return true if any change in the block output happened,
         *         false if not (recomputing didn't happen because it
         *         wasn't necesarry)
         */
  };

//----------------------------------------------------------------------

 /**
  * Represents a graphic texture designer block that generates and
  * stores image data.
  */

class c_graphic_block: public c_block

  {
    protected:
      t_color_buffer buffer;   /// image buffer of the block

    public:
      c_graphic_block();

        /**<
         * Class constructor, initialises a new object.
         */

      virtual void adjust();

        /**<
         * Adjusts the block parameters to current graph parameters
         * (i.e. resolution and so on)
         */

      virtual bool has_image();

        /**<
         * Checks if the given block can provide a color buffer with
         * image.
         *
         * @return true if this block has a color buffer, false
         *         otherwise
         */

      t_color_buffer *get_color_buffer();

        /**<
         * Returns a pointer to the block color buffer, in which the
         * image is stored.
         *
         * @return color buffer of the block
         */

      virtual ~c_graphic_block();

        /**<
         * Class destructor, destroys the block and frees all it's
         * memory.
         */
  };

//----------------------------------------------------------------------

 /**
  * Represents a special texture designer block that stores and
  * processes other than image data.
  */

class c_special_block: public c_block

  {
    protected:
    public:
      virtual void adjust();

        /**<
         * Adjusts the block parameters to current graph parameters
         * (i.e. resolution and so on)
         */

      virtual bool has_image();

        /**<
         * Checks if the given block can provide a color buffer with
         * image.
         *
         * @return true if this block has a color buffer, false
         *         otherwise
         */

      virtual ~c_special_block();

        /**<
         * Class destructor, destroys the block and frees all it's
         * memory.
         */
  };

//----------------------------------------------------------------------

 /**
  * Represents a texture description graph consisting of blocks. The
  * graph is oriented and must not contain cycles.
  */

class c_texture_graph

  {
    protected:
      unsigned int supersampling_level; /// supersampling level, 1 = off
      unsigned int resolution_x;        /** x resolution of the texture
                                            (final, supersampled) */
      unsigned int resolution_y;        /** y resolution of the texture
                                            (final, supersampled) */
      unsigned int last_id;             /// id to be assigned
      int random_seed;                  /// random number generator seed
      vector<c_block *> *blocks;        /// all the graph blocks
      vector<c_block *> *end_blocks;    /** blocks that are not an input
                                            of any other block */

    public:

      c_texture_graph();

        /**<
         * Class constructor, initialises a new texture graph.
         */

      ~c_texture_graph();

        /**<
         * Class destrucotr, deletes all the blocks and frees the
         * memory.
         */

      void clear();

        /**<
         * Deletes all block so that the graph is empty.
         */

      static string coordinations_to_string(
        double coordination_list[][2], unsigned int length);

        /**
          * Converts a list of coordinations to string representation.
          *
          * @param coordination_list array of [x,y] double values in
          *        range <0,1>
          * @param length length of the array (number of coordinations)
          *
          * @return string representation of the coordination list
          */

      static void string_to_coordinations(
        double coordination_list[][2], string coordinations,
        unsigned int *length, unsigned int max_length);

        /**
          * Converts a string representation of two-dimensional
          * coordinations to double array.
          *
          * @param coordination_list array of [x,y] double values in
          *        which the result will be stored
          * @param coordinations string representation of the
          *        coordination list
          * @param length in this variable the length of the list loaded
          *        will be stored
          * @param max_length specifies the maximum length that should
          *        be allowed to load
          */

      static string char_array_to_string(unsigned char char_array[],
        unsigned int length);

        /**
          * Converts an array of unsigned char values to its string
          * representation (decimal values separated by comma).
          *
          * @param char_array char array to be converted
          * @param length length of the array
          *
          * @return string representation of the char array
          */

      static void string_to_char_array(unsigned char char_array[],
        string char_string, unsigned int *length,
        unsigned int max_length);

        /**<
         * Converts the string representation of array of unsigned char
         * values (decimal values separated by comma) to char array.
         *
         * @param char_array in this variable the char array will be
         *        returned, the whole array will be filles with zeros
         *        initially within the max_length length
         * @param char_string string representation to be converted
         * @param length in this variable the length of the array loaded
         *        will be returned
         * @param max_length maximum allowed array length
         */

      static void string_to_double_array(double double_array[],
        string double_string, unsigned int *length,
        unsigned int max_length);

        /**<
         * Converts the string representation of array of double values
         * (decimal values separated by comma) to double array.
         *
         * @param double_array in this variable the double array will be
         *        returned, the whole array will be filles with zeros
         *        initially within the max_length length
         * @param double_string string representation to be converted
         * @param length in this variable the length of the array loaded
         *        will be returned
         * @param max_length maximum allowed array length
         */

      void block_invalidated(c_block *block);
      
        /**<
         * Informs the graph that a block was invalidated so that all
         * its children will be invalidated too.
         * 
         * @param block block that has been invalidated
         */

      void update();

        /**<
         * Updates the texture graph. Should be called after connect,
         * disconnect, delete etc. functions has been called.
         */

      bool compute(bool force,void (*progress_function)(int,int));

        /**<
         * Computes all the block outputs and executes all asociated
         * actions. As many as possible of the blocks will be computed,
         * even if there are errors. Optional pointer to function can
         * be provided to be updated about computation progress.
         *
         * @param force if true, all blocks will be forced to recompute
         *        even if it's not necesarry, otherwise only invalid
         *        blocks will be recomputed
         * @param progress_function pointer to function taking two int
         *        parameters that will be called every time a single
         *        block is to be computed (and after all blocks are
         *        computed), parameters passed to the function are
         *        number of blocks left to be computed and an id of the
         *        block to be computed next (in the last function call
         *        this is -1), this parameter can be NULL in which case
         *        it will be ignored 
         * 
         * @return true if every block was computed succesfully,
         *         otherwise false
         */

      bool compute(bool force);

        /**<
         * Computes all the block outputs and executes all asociated
         * actions. As many as possible of the blocks will be computed,
         * even if there are errors.
         *
         * @param force if true, all blocks will be forced to recompute
         *        even if it's not necesarry, otherwise only invalid
         *        blocks will be recomputed
         *
         * @return true if every block was computed succesfully,
         *         otherwise false
         */
         
      bool compute();

        /**<
         * Computes all the block outputs and executes all asociated
         * actions. This is equal to calling compute(false).
         *
         * @return true if every block was computed succesfully,
         *         otherwise false
         */

      bool connect_by_id(int id_input, int id_to, unsigned int slot);

        /**<
         * Connects two blocks identified by their IDs. If any of the
         * blocks with given IDs doesn't exist, nothing happens.
         *
         * @param id_input ID of the block to be connected to block with
         *        id_to ID
         * @param id_to ID of the block to which the block with id_input
         *        ID will be connected
         * @param slot slot number to connect the block to
         *
         * @return true if the blocks were connected, false if not
         */

      bool is_error();

        /**<
         * Checks if there is any error in any block of the graph.
         *
         * @return true if there is at least one error in any block,
         *         false otherwise
         */

      void invalidate_all();

        /**<
         * Invalidates all the block so their outputs will be forced to
         * be recomputed next time the compute function is called.
         */

      void adjust_all();

        /**<
         * Adjusts all the blocks so that their parameters are in match
         * with current graph parameters.
         */

      void set_supersampling(unsigned int level);

        /**<
         * Sets the texture supersapling level.
         *
         * @param level level of supersampling, 1 means no
         *        supersampling, 2 means 2 x 2 etc. Maximum value is
         *        MAX_SUPERSAMPLING.
         */

      void set_resolution(unsigned int x, unsigned int y);

        /**<
         * Sets the texture resolution.
         *
         * @param x horizontal texture resolution in pixels
         * @param y vertical texture resolution in pixels
         */

      void set_random_seed(int value);

        /**<
         * Sets the random seed value that affects random actions during
         * texture computation. Different values will result in slightly
         * different textures.
         *
         * @param value value to be set as seed
         */

      unsigned int get_supersampling();

        /**<
         * Gets the information about supersampling level set for the
         * texture.
         *
         * @return supersampling level
         */

      void get_resolution(unsigned int *x, unsigned int *y);

        /**<
         * Gets the information about texture resolution.
         *
         * @param x in this variable the horizontal resolution value in
         *        pixels will be returned
         * @param y in this variable the vertical resolution value in
         *        pixels will be returned
         */

      int get_random_seed();

        /**<
         * Gets the information about random seed value. This value
         * affects random actions during texture computation.
         *
         * @return random seed value
         */

      unsigned int get_number_of_blocks();

        /**<
         * Returns number of block in texture graph.
         *
         * @return number of block in texture graph
         */
         
      unsigned int get_number_of_invalid();
      
        /**<
         * Returns a number of invalid blocks in the graph.
         * 
         * @return number of invalid blocks in the graph.
         */

      c_block *get_block(unsigned int block_number);

        /**<
         * Returns a block of the texture graph with given index.
         *
         * @param block_number index of block that should be returned
         *        (note that this is not the block ID!)
         *
         * @return pointer to block at given index or NULL if the index
         *         is out of range
         */

      c_block *get_block_by_id(unsigned int block_id);

        /**<
         * Returns a block of the texture graph with given ID.
         *
         * @param block_id ID of block that should be returned
         *        (note that this is not the block index!)
         *
         * @return pointer to block with given ID or NULL if block with
         *         such ID doesn't exist
         */

      int add_block(c_block *block);

        /**<
         * Adds given block to texture graph. This will initialise the
         * block's parameters to default values and adjust them so that
         * they match the texture graph settings.
         *
         * @param block block to be added
         * 
         * @return id assigned to block
         */

      void remove_block(unsigned int block_number);

        /**<
         * Removes block with given index from texture graph. This
         * doesn't delete the block, it will just no longer belong to
         * the texture graph.
         *
         * @param block_number index of block to be removed from the
         *        texture graph
         */

      void delete_block(unsigned int block_number);

        /**<
         * Removes block with given index from texture graph and deletes
         * it.
         *
         * @param block_number index of block to be removed and deleted
         */
         
      void delete_block_with_id(unsigned int block_id);

        /**<
         * Removes block with given id from texture graph and deletes
         * it.
         *
         * @param block_number index of block to be removed and deleted
         */

      bool load_from_file(string filename);

        /**<
         * Loads the graph from XML file.
         *
         * @param filename name of the XML file
         *
         * @return true if the graph was loaded succesfully, false
         *         otherwise
         */

      bool save_to_file(string filename);

        /**<
         * Saves the graph as an XML file.
         *
         * @param filename name of the file to be saved
         *
         * @return true if the file was saved succesfully, false
         *         otherwise
         */

      void print_as_text();

        /**<
         * Debugging purposes method. Prints the graph text
         * representation to standard output.
         */
  };

/*======================================================================

                     concrete block definitions:

======================================================================*/

  /**
   * Voronoi diagram block.
   */

class c_block_voronoi_diagram: public c_graphic_block

  {
    #define VORONOI_DIAGRAM_NAME "voronoi diagram"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Color fill block that generates an image filled with constant
   * color.
   */

class c_block_color_fill: public c_graphic_block

  {
    #define COLOR_FILL_NAME "color fill"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Bump noise block.
   */

class c_block_bump_noise: public c_graphic_block

  {
    #define BUMP_NOISE_NAME "bump noise"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Fault formation noise block.
   */

class c_block_fault_formation_noise: public c_graphic_block

  {
    #define FAULT_FORMATION_NOISE_NAME "fault formation noise"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Perlin noise block.
   */

class c_block_perlin_noise: public c_graphic_block

  {
    #define PERLIN_NOISE_NAME "perlin noise"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Blur block.
   */

class c_block_blur: public c_graphic_block

  {
    #define BLUR_NAME "blur"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Geometric transformation block. Performs rotation, shift and flip
   * operations in this order.
   */

class c_block_geometric_transform: public c_graphic_block

  {
    #define GEOMETRIC_TRANSFORM_NAME "geometric transform"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Simple white noise block.
   */

class c_block_simple_noise: public c_graphic_block

  {
    #define SIMPLE_NOISE_NAME "simple noise"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Replace colors block.
   */

class c_block_replace_colors: public c_graphic_block

  {
    #define REPLACE_COLORS_NAME "replace colors"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Tile block.
   */

class c_block_tile: public c_graphic_block

  {
    #define TILE_NAME "tile"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Channel mix block.
   */

class c_block_mix_channels: public c_graphic_block

  {
    #define MIX_CHANNELS_NAME "mix channels"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * RGB adjust block.
   */

class c_block_rgb: public c_graphic_block

  {
    #define RGB_NAME "adjust rgb"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * HSL adjust block.
   */

class c_block_hsl: public c_graphic_block

  {
    #define HSL_NAME "adjust hsl"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Substrate algorithm block.
   */

class c_block_substrate: public c_graphic_block

  {
    #define SUBSTRATE_NAME "substrate"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Mixes or blends two images together with optional alpha channel
   * spicified with third image.
   */

class c_block_mix: public c_graphic_block

  {
    #define MIX_NAME "mix"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Marble block.
   */

class c_block_marble: public c_graphic_block

  {
    #define MARBLE_NAME "marble"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Wood block.
   */

class c_block_wood: public c_graphic_block

  {
    #define WOOD_NAME "wood"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Particle block.
   */

class c_block_particles: public c_graphic_block

  {
    #define PARTICLES_NAME "particles"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Circle transform block.
   */

class c_block_circle_transform: public c_graphic_block

  {
    #define CIRCLE_TRANSFORM_NAME "circle transform"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Sine transform block.
   */

class c_block_sine_transform: public c_graphic_block

  {
    #define SINE_TRANSFORM_NAME "sine transform"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Rock paper scissors cellular automaton block.
   */

class c_block_cellular_automaton_rps: public c_graphic_block

  {
    #define CELLULAR_RPS_NAME "cellular automaton rps"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Cyclic cellular automaton block.
   */

class c_block_cellular_automaton_cyclic: public c_graphic_block

  {
    #define CELLULAR_CYCLIC_NAME "cellular automaton cyclic"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * General cellular automaton block.
   */

class c_block_cellular_automaton_general: public c_graphic_block

  {
    #define CELLULAR_GENERAL_NAME "cellular automaton general"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Dither block.
   */

class c_block_dither: public c_graphic_block

  {
    #define DITHER_NAME "dither"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Amplitude crop block.
   */

class c_block_crop_amplitude: public c_graphic_block

  {
    #define CROP_AMPLITUDE_NAME "crop amplitude"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Color invert block.
   */

class c_block_invert: public c_graphic_block

  {
    #define INVERT_NAME "invert"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Radius transform block.
   */

class c_block_radius_transform: public c_graphic_block

  {
    #define RADIUS_TRANSFORM_NAME "radius transform"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * End block, stores the final texture in set resolution and with
   * supersampling applied. It is mean to use in applications to
   * access the final texture.
   */

class c_block_end: public c_graphic_block

  {
    #define END_BLOCK_NAME "end"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Normal map block.
   */

class c_block_normal_map: public c_graphic_block

  {
    #define NORMAL_MAP_NAME "normal map"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Lighting block.
   */

class c_block_light: public c_graphic_block

  {
    #define LIGHT_NAME "light"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Glass block.
   */

class c_block_glass: public c_graphic_block

  {
    #define GLASS_NAME "glass"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Grayscale block.
   */

class c_block_grayscale: public c_graphic_block

  {
    #define GRAYSCALE_NAME "grayscale"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Turtle block. Can draw L-Systems.
   */

class c_block_turtle: public c_graphic_block

  {
    #define TURTLE_NAME "turtle"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Square mosaic block.
   */

class c_block_square_mosaic: public c_graphic_block

  {
    #define SQUARE_MOSAIC_NAME "square mosaic"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Convolution block.
   */

class c_block_convolution: public c_graphic_block

  {
    #define CONVOLUTION_NAME "convolution"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Edge detection block.
   */

class c_block_edge_detection: public c_graphic_block

  {
    #define EDGE_DETECTION_NAME "edge detection"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Adjust brightness/contrast block.
   */

class c_block_brightness_contrast: public c_graphic_block

  {
    #define BRIGHTNESS_CONTRAST_NAME "brightness and contrast"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Sharpen block.
   */

class c_block_sharpen: public c_graphic_block

  {
    #define SHARPEN_NAME "sharpen"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Emboss block.
   */

class c_block_emboss: public c_graphic_block

  {
    #define EMBOSS_NAME "emboss"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

  /**
   * Map to color transition block.
   */

class c_block_map_transition: public c_graphic_block

  {
    #define MAP_TRANSITION_NAME "map transition"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

 /**
  * A block that loads an image from a file.
  */

class c_block_file_load: public c_graphic_block

  {
    #define FILE_LOAD_NAME "load file"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

 /**
  * Special block that saves its input to png file in set resolution
  * and with supersampling applied.
  */

class c_block_file_save: public c_special_block

  {
    #define FILE_SAVE_NAME "save file"

    protected:
      virtual void set_default();
      virtual bool execute();
  };

//----------------------------------------------------------------------

 /**
  * Special block that stores a color transition.
  */

class c_block_color_transition: public c_special_block

  {
    #define COLOR_TRANSITION_NAME "color transition"

    protected:
      t_color_transition transition;
      virtual void set_default();
      virtual bool execute();

    public:
      c_block_color_transition();
      ~c_block_color_transition();
      
      void get_color(unsigned int coordination, unsigned char *r,
        unsigned char *g, unsigned char *b);
      
      /**<
       * Returns a color with given coordination of the block's
       * color transition.
       * 
       * @param coordination point coordination
       * @param r in this variable an amount of red will be returned
       * @param g in this variable an amount of green will be returned
       * @param b in this variable an amount of blue will be returned
       */
      
      t_color_transition *get_color_transition();

      /**<
       * Returns a pointer to a color transition stored by this block.
       *
       * @return pointer to this block's color transition
       */
  };

//----------------------------------------------------------------------

 /**
  * Special block that stores an L-system.
  */

class c_block_l_system: public c_special_block

  {
    #define L_SYSTEM_NAME "l-system"

    protected:
      t_grammar grammar;
      virtual void set_default();
      virtual bool execute();

    public:
      c_block_l_system();
      ~c_block_l_system();
      t_grammar *get_grammar();

      /**<
       * Returns a pointer to a grammar stored by this block.
       *
       * @return pointer to this block's grammar
       */
  };

//----------------------------------------------------------------------

} // namespace

#endif
