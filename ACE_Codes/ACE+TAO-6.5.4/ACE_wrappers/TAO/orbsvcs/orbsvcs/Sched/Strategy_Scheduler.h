// -*- C++ -*-

//=============================================================================
/**
 *  @file    Strategy_Scheduler.h
 *
 *  @author Chris Gill
 */
//=============================================================================


#ifndef STRATEGY_SCHEDULER_H
#define STRATEGY_SCHEDULER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Sched/DynSched.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// forward declaration of the abstract base class for scheduler strategies
class ACE_Scheduler_Strategy;

/////////////////////////////////
// Strategized scheduler class //
/////////////////////////////////

/**
 * @class ACE_Strategy_Scheduler
 *
 * @brief ACE_Strategy_Scheduler
 *
 * Strategized scheduler implementation.  Provides an implementation
 * of all strategy specific scheduling mechanisms, which relies on the
 * methods of the associated strategy class.
 */
class TAO_RTSched_Export ACE_Strategy_Scheduler : public ACE_DynScheduler
{
// public interface
public:

    /// = Constructor.
  ACE_Strategy_Scheduler (ACE_Scheduler_Strategy &strategy);

    /// = Virtual destructor.
  virtual ~ACE_Strategy_Scheduler ();

    /// = Assigns priorities to the sorted dispatch schedule,
    ///   according to the strategy's priority comparison operator.
  status_t assign_priorities (
    Dispatch_Entry **dispatches, u_int count,
    ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set);

    /// = Assigns dynamic and static sub-priorities to the sorted dispatch
    ///   schedule, according to the strategy's subpriority comparisons.
  status_t assign_subpriorities (
    Dispatch_Entry **dispatches, u_int count,
    ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set);

    /// = Determine the minimum critical priority number.
  virtual Preemption_Priority minimum_critical_priority ();

private:

    /// = Schedules a dispatch entry into the timeline being created.
  virtual status_t schedule_timeline_entry (Dispatch_Entry &dispatch_entry,
                                            ACE_Unbounded_Queue <Dispatch_Entry *>
                                              &reschedule_queue);

    /// = Sets up the schedule in the order generated by the strategy.
  virtual status_t sort_dispatches (Dispatch_Entry **dispatches, u_int count);

    /// = Strategy for comparing and sorting dispatch entries.
  ACE_Scheduler_Strategy &strategy_;

  ACE_Strategy_Scheduler (const ACE_Strategy_Scheduler &);
  ACE_Strategy_Scheduler &operator= (const ACE_Strategy_Scheduler &);
};



////////////////////////////////////////
// Factory for strategized schedulers //
////////////////////////////////////////

/**
 * @class ACE_Strategy_Scheduler_Factory
 *
 * @brief ACE_Strategy_Scheduler_Factory
 *
 * Provides a type parameterized factory method that constructs
 * and returns a scheduler that uses the given scheduling strategy
 */
template <class STRATEGY>
class ACE_Strategy_Scheduler_Factory
{
public:

    /// = Constructs and returns a scheduler strategized with
    ///   an instance of the the parameterized strategy type.
        static ACE_Strategy_Scheduler * create (RtecScheduler::Preemption_Priority_t minimum_critical_priority);
};


//////////////////////////
// Scheduler Strategies //
//////////////////////////

/**
 * @class ACE_Scheduler_Strategy
 *
 * @brief ACE_Scheduler_Strategy
 *
 * Abstract Base Class for scheduling strategies: each derived class
 * must define an ordering strategy for dispatch entries based on a
 * specific scheduling algorithm.
 */
class TAO_RTSched_Export ACE_Scheduler_Strategy
{
public:

  /// = Constructor.
  ACE_Scheduler_Strategy (ACE_DynScheduler::Preemption_Priority minimum_critical_priority = 0);

  /// Destructor.
  virtual ~ACE_Scheduler_Strategy (void);

    /**
     * = Compares two dispatch entries in strategy specific high to low
     *   priority ordering: returns -1 if the first Dispatch_Entry is greater
     *   in the order, 0 if they are equivalent, or 1 if the second
     *   Dispatch_Entry is greater in the order.
     */
  virtual int priority_comp (const Dispatch_Entry &first_entry,
                             const Dispatch_Entry &second_entry) = 0;

    /// = Sorts the dispatch entry link pointer array according to
    ///   the specific sort order defined by the strategy.
  virtual void sort (Dispatch_Entry **dispatch_entries,
                     u_int count) = 0;

    /// = Determines the minimum critical priority number.
  virtual ACE_DynScheduler::Preemption_Priority minimum_critical_priority ();

    /**
     * = Compares two dispatch entries in strategy specific high to low
     *   dynamic subpriority ordering: returns -1 if the first Dispatch_Entry
     *   is greater in the order, 0 if they are equivalent, or 1 if the
     *   second Dispatch_Entry is greater in the order.
     */
  virtual int dynamic_subpriority_comp (const Dispatch_Entry &first_entry,
                                        const Dispatch_Entry &second_entry) = 0;

    /// = Returns a dynamic subpriority value
    ///   for the given timeline entry at the current time.
    virtual long dynamic_subpriority (Dispatch_Entry &entry,
                                      RtecScheduler::Time current_time) = 0;

    /**
     * = Provides a lowest level ordering based first on importance
     * (descending), and then on the dependency topological sort finishing
     * time (ascending).
     */
  virtual int static_subpriority_comp (const Dispatch_Entry &first_entry,
                                       const Dispatch_Entry &second_entry);

    /// = Provide the dispatching queue type for the given dispatch entry.
  virtual ACE_DynScheduler::Dispatching_Type
    dispatch_type (const Dispatch_Entry &entry) = 0;

protected:

    /**
     * = Compares two dispatch entries using the specific priority, dynamic
     *   subpriority, and static subpriority method definitions provided by
     *   the derived strategy class to produce the strategy specific sort
     *   ordering: returns -1 if the first Dispatch_Entry is greater in the
     *   order, 0 if they are equivalent, or 1 if the second Dispatch_Entry is
     *   greater in the order.  This is an example of the Template Method
     *   pattern (and also of Pree's Unification Metapattern), in which
     *   derived classes provide  definitions of the methods on which the
     *   sort_comp Template Method relies.
     */
  int sort_comp (const Dispatch_Entry &first_entry,
                 const Dispatch_Entry &second_entry);

    /// = The minimum critical priority number for the strategy.
  ACE_DynScheduler::Preemption_Priority minimum_critical_priority_;
};



/**
 * @class ACE_MUF_Scheduler_Strategy
 *
 * @brief ACE_MUF_Scheduler_Strategy
 *
 * Defines "schedule" method using Maximum Urgency First
 * scheduling algorithm.
 */
class TAO_RTSched_Export ACE_MUF_Scheduler_Strategy : public ACE_Scheduler_Strategy
{
public:

    /// = Constructor.
  ACE_MUF_Scheduler_Strategy (ACE_DynScheduler::Preemption_Priority minimum_critical_priority = 0);

  /// = Virtual destructor.
  virtual ~ACE_MUF_Scheduler_Strategy ();

    /// = Returns an instance of the strategy.
  static ACE_MUF_Scheduler_Strategy *instance ();

    /**
     * = Compares two dispatch entries by maximum criticality: returns -1 if
     *   the first Dispatch_Entry is greater in the order, 0 if they're
     *   equivalent, or 1 if the second Dispatch_Entry is greater in the order.
     */
  virtual int priority_comp (const Dispatch_Entry &first_entry,
                             const Dispatch_Entry &second_entry);

    /// = Sorts the dispatch entry link pointer array
    ///   in descending urgency order.
  virtual void sort (Dispatch_Entry **dispatch_entries,
                     u_int count);

    /// = Determines the minimum critical priority number.
  virtual ACE_DynScheduler::Preemption_Priority minimum_critical_priority ();

    /// = Provides the dispatching queue type for the given dispatch entry.
  virtual ACE_DynScheduler::Dispatching_Type
    dispatch_type (const Dispatch_Entry &entry);

protected:

    /**
     * = Returns a dynamic subpriority value at the current time for
     *   the given timeline entry: if the operation has
     *   non-negative laxity, then the value is positive, and a lower
     *   laxity gives a higher dynamic subpriority; if the operation
     *   has negative laxity, the value is the (negative) laxity value.
     */
    virtual long dynamic_subpriority (Dispatch_Entry &entry,
                                      RtecScheduler::Time current_time);

    /**
     * = Orders two dispatch entries by ascending laxity: returns -1 if the
     *   first Dispatch_Entry is greater in the order, 0 if they're equivalent,
     *   1 if the second Dispatch_Entry is greater in the order.
     */
  virtual int dynamic_subpriority_comp (
    const Dispatch_Entry &first_entry,
    const Dispatch_Entry &second_entry);

private:

    // = Comparison function to pass to qsort: calls instance ()->sort_comp ().
#if defined (ACE_HAS_WINCE)
  static int _cdecl sort_function (void *arg1, void *arg2);
#else
  static int sort_function (void *arg1, void *arg2);
#endif  // ACE_HAS_WINCE

    /// Instance of the strategy.
  static ACE_MUF_Scheduler_Strategy *instance_;
};


/**
 * @class ACE_RMS_Scheduler_Strategy
 *
 * @brief ACE_RMS_Scheduler_Strategy
 *
 * Defines "schedule" method using Rate Monotonic
 * Scheduling algorithm.
 */
class TAO_RTSched_Export ACE_RMS_Scheduler_Strategy : public ACE_Scheduler_Strategy
{
public:

    /// = Constructor.
  ACE_RMS_Scheduler_Strategy (ACE_DynScheduler::Preemption_Priority minimum_critical_priority = 0);

    /// = Virtual destructor.
  virtual ~ACE_RMS_Scheduler_Strategy ();

    /// Returns an instance of the strategy.
  static ACE_RMS_Scheduler_Strategy *instance ();

    /**
     * = Compares two dispatch entries by minimum period: returns -1 if the
     *   first Dispatch_Entry is greater in the order, 0 if they're equivalent,
     *   or 1 if the second Dispatch_Entry is greater in the order.
     */
  virtual int priority_comp (const Dispatch_Entry &first_entry,
                             const Dispatch_Entry &second_entry);

    /// = Sorts the dispatch entry link pointer array in
    ///   descending RMS (rate) order.
  virtual void sort (Dispatch_Entry **dispatch_entries,
                     u_int count);

    /// = Determine the minimum critical priority number.
  virtual ACE_DynScheduler::Preemption_Priority minimum_critical_priority ();

    /// = Provide the dispatching queue type for the given dispatch entry.
  virtual ACE_DynScheduler::Dispatching_Type
    dispatch_type (const Dispatch_Entry &entry);

protected:

    /// = Just returns 0: all operations have
    ///   the same dynamic subpriority value.
    virtual long dynamic_subpriority (Dispatch_Entry &entry,
                                      RtecScheduler::Time current_time);

    /// = All dispatches in a given priority level have the same dynamic
    ///   subpriority under RMS: just returns 0.
  virtual int dynamic_subpriority_comp
    (const Dispatch_Entry &first_entry,
     const Dispatch_Entry &second_entry);

private:

    /// = Comparison function to pass to qsort: calls instance ()->sort_comp ().
  static int sort_function (void *arg1, void *arg2);

    /// = Instance of the strategy.
  static ACE_RMS_Scheduler_Strategy *instance_;

};



/**
 * @class ACE_MLF_Scheduler_Strategy
 *
 * @brief ACE_MLF_Scheduler_Strategy
 *
 * Defines "schedule" method using Minimum Laxity First
 * scheduling algorithm.
 */
class TAO_RTSched_Export ACE_MLF_Scheduler_Strategy : public ACE_Scheduler_Strategy
{
public:

    /// = Constructor.
  ACE_MLF_Scheduler_Strategy (ACE_DynScheduler::Preemption_Priority minimum_critical_priority = 0);

    /// = Virtual destructor.
  virtual ~ACE_MLF_Scheduler_Strategy ();

    /// = Returns an instance of the strategy.
  static ACE_MLF_Scheduler_Strategy *instance ();

    /// = Just returns 0, as all dispatch entries are of equivalent
    ///   static priority under MLF.
  virtual int priority_comp (const Dispatch_Entry &first_entry,
                             const Dispatch_Entry &second_entry);

    /// = Sorts the dispatch entry link pointer array in ascending laxity order.
  virtual void sort (Dispatch_Entry **dispatch_entries,
                     u_int count);

  /// = Provide the dispatching queue type for the given dispatch entry.
  virtual ACE_DynScheduler::Dispatching_Type
    dispatch_type (const Dispatch_Entry &entry);

protected:

    /**
     * = Returns a dynamic subpriority value at the current time for
     *   the given timeline entry: if the operation has
     *   non-negative laxity, then the value is positive, and a lower
     *   laxity gives a higher dynamic subpriority; if the operation
     *   has negative laxity, the value is the (negative) laxity value.
     */
    virtual long dynamic_subpriority (Dispatch_Entry &entry,
                                      RtecScheduler::Time current_time);

    /**
     * = Orders two dispatch entries by ascending laxity: returns -1 if the
     *   first Dispatch_Entry is greater in the order, 0 if they're equivalent,
     *   or 1 if the second Dispatch_Entry is greater in the order.
     */
  virtual int dynamic_subpriority_comp
    (const Dispatch_Entry &first_entry,
     const Dispatch_Entry &second_entry);

private:

    /// = Comparison function to pass to qsort: calls instance ()->sort_comp ().
  static int sort_function (void *arg1, void *arg2);

    /// = Instance of the strategy
  static ACE_MLF_Scheduler_Strategy *instance_;

};


/**
 * @class ACE_EDF_Scheduler_Strategy
 *
 * @brief ACE_EDF_Scheduler_Strategy
 *
 * Defines "schedule" method using Earliest Deadline First
 * scheduling algorithm.
 */
class TAO_RTSched_Export ACE_EDF_Scheduler_Strategy : public ACE_Scheduler_Strategy
{
public:

    /// = Default constructor.
  ACE_EDF_Scheduler_Strategy (ACE_DynScheduler::Preemption_Priority minimum_critical_priority = 0);

    /// = Virtual destructor.
  virtual ~ACE_EDF_Scheduler_Strategy ();

    /// = Returns an instance of the strategy.
  static ACE_EDF_Scheduler_Strategy *instance ();

    /// = Returns 0, as all dispatch entries are of equivalent
    ///   priority under EDF.
  virtual int priority_comp (const Dispatch_Entry &first_entry,
                             const Dispatch_Entry &second_entry);

    /// = Sorts the dispatch entry link pointer array
    ///   in ascending deadline (period) order.
  virtual void sort (Dispatch_Entry **dispatch_entries,
                     u_int count);

  /// = Provide the dispatching queue type for the given dispatch entry.
  virtual ACE_DynScheduler::Dispatching_Type
    dispatch_type (const Dispatch_Entry &entry);

protected:

    /**
     * = Returns a dynamic subpriority value at the current time for the
     *   given timeline entry: if the operation has non-negative
     *   time to deadline, then value is positive, and a shorter time to
     *   deadline gives a higher dynamic subpriority; if the operation has a
     *   negative time to deadline, the value is (negative) time to deadline.
     */
    virtual long dynamic_subpriority (Dispatch_Entry &entry,
                                      RtecScheduler::Time current_time);


    /**
     * = Orders two dispatch entries by ascending time to deadline: returns -1
     *   if the first Dispatch_Entry is greater in the order, 0 if they're
     *   equivalent, or 1 if the second Dispatch_Entry is greater in the order.
     */
  virtual int dynamic_subpriority_comp
    (const Dispatch_Entry &first_entry,
     const Dispatch_Entry &second_entry);

private:

    /// = Comparison function to pass to qsort: calls instance ()->sort_comp ().
  static int sort_function (void *arg1, void *arg2);

    /// = Instance of the strategy.
  static ACE_EDF_Scheduler_Strategy *instance_;

};


/**
 * @class ACE_Criticality_Scheduler_Strategy
 *
 * @brief ACE_Criticality_Scheduler_Strategy
 *
 * Defines "schedule" method using a simple mapping directly from
 * operation criticality to static priority.
 */
class TAO_RTSched_Export ACE_Criticality_Scheduler_Strategy : public ACE_Scheduler_Strategy
{
public:

    /// = Constructor.
  ACE_Criticality_Scheduler_Strategy (ACE_DynScheduler::Preemption_Priority minimum_critical_priority = 0);

    /// = Virtual destructor.
  virtual ~ACE_Criticality_Scheduler_Strategy ();

    /// = Returns an instance of the strategy.
  static ACE_Criticality_Scheduler_Strategy *instance ();

    /**
     * = Compares two dispatch entries by minimum period: returns -1 if the
     *   first Dispatch_Entry is greater in the order, 0 if they're equivalent,
     *   or 1 if the second Dispatch_Entry is greater in the order.
     */
  virtual int priority_comp (const Dispatch_Entry &first_entry,
                             const Dispatch_Entry &second_entry);

    /// = Sort the dispatch entry link pointer array in descending
    ///   criticality order.
  virtual void sort (Dispatch_Entry **dispatch_entries,
                     u_int count);

    /// = Determine the minimum critical priority number.
  virtual ACE_DynScheduler::Preemption_Priority minimum_critical_priority ();

  /// Provide the dispatching queue type for the given dispatch entry.
  virtual ACE_DynScheduler::Dispatching_Type
    dispatch_type (const Dispatch_Entry &entry);

protected:

    /// = Just returns 0: all operations have
    ///   the same dynamic subpriority value.
    virtual long dynamic_subpriority (Dispatch_Entry &entry,
                                      RtecScheduler::Time current_time);

    /// = All dispatches in a given priority level have the same dynamic
    ///   subpriority under this strategy: just returns 0.
  virtual int dynamic_subpriority_comp
    (const Dispatch_Entry &first_entry,
     const Dispatch_Entry &second_entry);

private:

    /// = Comparison function to pass to qsort: calls instance ()->sort_comp ().
  static int sort_function (void *arg1, void *arg2);

    /// = Instance of the strategy.
  static ACE_Criticality_Scheduler_Strategy *instance_;

};


TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* STRATEGY_SCHEDULER_H */

// EOF
