/**
 * @file DAQProcess class interface
 *
 * DAQProcess is the central container for instantiated DAQModules and Queues
 * within a DAQ Application. It loads a ModuleList which defines the graph of
 * DAQModules and Queues and any command ordering. DAQProcess is responsible
 * for distributing commands recieved from CCM to the DAQModules in the order
 * defined.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_DAQPROCESS_HH_
#define APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_DAQPROCESS_HH_

#include "app-framework-base/Queues/QueueI.hh"
#include "app-framework-base/Core/ModuleList.hh"
#include "app-framework-base/DAQModules/DAQModuleI.hh"

#include <list>
#include <map>
#include <memory>
#include <string>

namespace appframework {
/**
 * @brief The DAQProcess class is the central container for DAQModules and
 * Queues.
 *
 * DAQProcess receives commands from CCM and distributes them to the DAQModules
 * in the order defined in the CommandOrderMap received from the ModuleList
 * during register_modules.
 */
class DAQProcess {
public:
  /**
   * @brief DAQProcess Constructor
   * @param args Command-line arguments to the DAQ Application
   *
   * The DAQProcess constructor instantiates essential DAQ Application services.
   * Services are passed the command-line options and may also read basic
   * configuration from the environment.
   */
  explicit DAQProcess(std::list<std::string> args);
  /**
   * @brief Using the given ModuleList, construct the graph of DAQModules and
   * Queues
   * @param ml ModuleList to call ModuleList::ConstructGraph on
   *
   * The register_modules function calls the ModuleList's ConstructGraph
   * function, which instantiates and links together the DAQModules and Queues
   * needed by this DAQ Application. ConstructGraph also defines any ordering of
   * commands for DAQModules.
   */
  void register_modules(ModuleList &ml);
  /**
   * @brief Execute the specified command on the loaded DAQModules
   * @param cmd Command to execute
   *
   * This function will determine if there is an entry in the command order map
   * for this command, and if so, first send the command to the DAQModules in
   * that list in the order specified. Then, any remaining DAQModules will
   * receive the command in an unspecified order.
   */
  void execute_command(std::string cmd);
  /**
   * @brief Start the CommandFacility listener
   * @return Return code from listener
   *
   * This function should call the loaded CommandFacility::listen method, which
   * should block for the duration of the DAQ Application, calling
   * execute_command as necessary.
   */
  int listen();

  DAQProcess(const DAQProcess &) = delete;
  DAQProcess &operator=(const DAQProcess &) = delete;
  DAQProcess(DAQProcess &&) = delete;
  DAQProcess &operator=(DAQProcess &&) = delete;

private:
  QueueMap queueMap_;             ///< String alias for each Queue
  DAQModuleMap daqModuleMap_;     ///< String alias for each DAQModule
  CommandOrderMap commandOrderMap_; ///< Order DAQModule commands by alias
};
} // namespace appframework

#endif // APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_DAQPROCESS_HH_
