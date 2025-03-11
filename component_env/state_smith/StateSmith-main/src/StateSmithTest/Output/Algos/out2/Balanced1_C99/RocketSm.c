// Autogenerated with StateSmith.
// Algorithm: Balanced1. See https://github.com/StateSmith/StateSmith/wiki/Algorithms

// RenderConfig.C.CFileTop

#include "RocketSm.h"
#include <stdbool.h> // required for `consume_event` flag
#include <string.h> // for memset

// This function is used when StateSmith doesn't know what the active leaf state is at
// compile time due to sub states or when multiple states need to be exited.
static void exit_up_to_state_handler(RocketSm* sm, RocketSm_Func desired_state_exit_handler);

static void ROOT_enter(RocketSm* sm);

static void ROOT_exit(RocketSm* sm);

static void GROUP_enter(RocketSm* sm);

static void GROUP_exit(RocketSm* sm);

static void GROUP_ev1(RocketSm* sm);

static void G1_enter(RocketSm* sm);

static void G1_exit(RocketSm* sm);

static void G1_ev1(RocketSm* sm);

static void G2_enter(RocketSm* sm);

static void G2_exit(RocketSm* sm);

static void G2_ev2(RocketSm* sm);

static void S1_enter(RocketSm* sm);

static void S1_exit(RocketSm* sm);


// State machine constructor. Must be called before start or dispatch event functions. Not thread safe.
void RocketSm_ctor(RocketSm* sm)
{
    memset(sm, 0, sizeof(*sm));
}

// Starts the state machine. Must be called before dispatching events. Not thread safe.
void RocketSm_start(RocketSm* sm)
{
    ROOT_enter(sm);
    // ROOT behavior
    // uml: TransitionTo(ROOT.<InitialState>)
    {
        // Step 1: Exit states until we reach `ROOT` state (Least Common Ancestor for transition). Already at LCA, no exiting required.
        
        // Step 2: Transition action: ``.
        
        // Step 3: Enter/move towards transition target `ROOT.<InitialState>`.
        // ROOT.<InitialState> is a pseudo state and cannot have an `enter` trigger.
        
        // ROOT.<InitialState> behavior
        // uml: TransitionTo(group)
        {
            // Step 1: Exit states until we reach `ROOT` state (Least Common Ancestor for transition). Already at LCA, no exiting required.
            
            // Step 2: Transition action: ``.
            
            // Step 3: Enter/move towards transition target `group`.
            GROUP_enter(sm);
            
            // group.<InitialState> behavior
            // uml: TransitionTo(g1)
            {
                // Step 1: Exit states until we reach `group` state (Least Common Ancestor for transition). Already at LCA, no exiting required.
                
                // Step 2: Transition action: ``.
                
                // Step 3: Enter/move towards transition target `g1`.
                G1_enter(sm);
                
                // Step 4: complete transition. Ends event dispatch. No other behaviors are checked.
                sm->state_id = RocketSm_StateId_G1;
                // No ancestor handles event. Can skip nulling `ancestor_event_handler`.
                return;
            } // end of behavior for group.<InitialState>
        } // end of behavior for ROOT.<InitialState>
    } // end of behavior for ROOT
}

// Dispatches an event to the state machine. Not thread safe.
// Note! This function assumes that the `event_id` parameter is valid.
void RocketSm_dispatch_event(RocketSm* sm, RocketSm_EventId event_id)
{
    RocketSm_Func behavior_func = sm->current_event_handlers[event_id];
    
    while (behavior_func != NULL)
    {
        sm->ancestor_event_handler = NULL;
        behavior_func(sm);
        behavior_func = sm->ancestor_event_handler;
    }
}

// This function is used when StateSmith doesn't know what the active leaf state is at
// compile time due to sub states or when multiple states need to be exited.
static void exit_up_to_state_handler(RocketSm* sm, RocketSm_Func desired_state_exit_handler)
{
    while (sm->current_state_exit_handler != desired_state_exit_handler)
    {
        sm->current_state_exit_handler(sm);
    }
}


////////////////////////////////////////////////////////////////////////////////
// event handlers for state ROOT
////////////////////////////////////////////////////////////////////////////////

static void ROOT_enter(RocketSm* sm)
{
    // setup trigger/event handlers
    sm->current_state_exit_handler = ROOT_exit;
}

static void ROOT_exit(RocketSm* sm)
{
    // State machine root is a special case. It cannot be exited. Mark as unused.
    (void)sm;
}


////////////////////////////////////////////////////////////////////////////////
// event handlers for state GROUP
////////////////////////////////////////////////////////////////////////////////

static void GROUP_enter(RocketSm* sm)
{
    // setup trigger/event handlers
    sm->current_state_exit_handler = GROUP_exit;
    sm->current_event_handlers[RocketSm_EventId_EV1] = GROUP_ev1;
}

static void GROUP_exit(RocketSm* sm)
{
    // adjust function pointers for this state's exit
    sm->current_state_exit_handler = ROOT_exit;
    sm->current_event_handlers[RocketSm_EventId_EV1] = NULL;  // no ancestor listens to this event
}

static void GROUP_ev1(RocketSm* sm)
{
    // No ancestor state handles `ev1` event.
    
    // group behavior
    // uml: EV1 TransitionTo(s1)
    {
        // Step 1: Exit states until we reach `ROOT` state (Least Common Ancestor for transition).
        exit_up_to_state_handler(sm, ROOT_exit);
        
        // Step 2: Transition action: ``.
        
        // Step 3: Enter/move towards transition target `s1`.
        S1_enter(sm);
        
        // Step 4: complete transition. Ends event dispatch. No other behaviors are checked.
        sm->state_id = RocketSm_StateId_S1;
        // No ancestor handles event. Can skip nulling `ancestor_event_handler`.
        return;
    } // end of behavior for group
}


////////////////////////////////////////////////////////////////////////////////
// event handlers for state G1
////////////////////////////////////////////////////////////////////////////////

static void G1_enter(RocketSm* sm)
{
    // setup trigger/event handlers
    sm->current_state_exit_handler = G1_exit;
    sm->current_event_handlers[RocketSm_EventId_EV1] = G1_ev1;
}

static void G1_exit(RocketSm* sm)
{
    // adjust function pointers for this state's exit
    sm->current_state_exit_handler = GROUP_exit;
    sm->current_event_handlers[RocketSm_EventId_EV1] = GROUP_ev1;  // the next ancestor that handles this event is GROUP
}

static void G1_ev1(RocketSm* sm)
{
    // Setup handler for next ancestor that listens to `ev1` event.
    sm->ancestor_event_handler = GROUP_ev1;
    
    // g1 behavior
    // uml: EV1 [a > 20] TransitionTo(g2)
    if (a > 20)
    {
        // Step 1: Exit states until we reach `group` state (Least Common Ancestor for transition).
        G1_exit(sm);
        
        // Step 2: Transition action: ``.
        
        // Step 3: Enter/move towards transition target `g2`.
        G2_enter(sm);
        
        // Step 4: complete transition. Ends event dispatch. No other behaviors are checked.
        sm->state_id = RocketSm_StateId_G2;
        sm->ancestor_event_handler = NULL;
        return;
    } // end of behavior for g1
}


////////////////////////////////////////////////////////////////////////////////
// event handlers for state G2
////////////////////////////////////////////////////////////////////////////////

static void G2_enter(RocketSm* sm)
{
    // setup trigger/event handlers
    sm->current_state_exit_handler = G2_exit;
    sm->current_event_handlers[RocketSm_EventId_EV2] = G2_ev2;
}

static void G2_exit(RocketSm* sm)
{
    // adjust function pointers for this state's exit
    sm->current_state_exit_handler = GROUP_exit;
    sm->current_event_handlers[RocketSm_EventId_EV2] = NULL;  // no ancestor listens to this event
}

static void G2_ev2(RocketSm* sm)
{
    // No ancestor state handles `ev2` event.
    
    // g2 behavior
    // uml: EV2 TransitionTo(g1)
    {
        // Step 1: Exit states until we reach `group` state (Least Common Ancestor for transition).
        G2_exit(sm);
        
        // Step 2: Transition action: ``.
        
        // Step 3: Enter/move towards transition target `g1`.
        G1_enter(sm);
        
        // Step 4: complete transition. Ends event dispatch. No other behaviors are checked.
        sm->state_id = RocketSm_StateId_G1;
        // No ancestor handles event. Can skip nulling `ancestor_event_handler`.
        return;
    } // end of behavior for g2
}


////////////////////////////////////////////////////////////////////////////////
// event handlers for state S1
////////////////////////////////////////////////////////////////////////////////

static void S1_enter(RocketSm* sm)
{
    // setup trigger/event handlers
    sm->current_state_exit_handler = S1_exit;
}

static void S1_exit(RocketSm* sm)
{
    // adjust function pointers for this state's exit
    sm->current_state_exit_handler = ROOT_exit;
}

// Thread safe.
char const * RocketSm_state_id_to_string(RocketSm_StateId id)
{
    switch (id)
    {
        case RocketSm_StateId_ROOT: return "ROOT";
        case RocketSm_StateId_GROUP: return "GROUP";
        case RocketSm_StateId_G1: return "G1";
        case RocketSm_StateId_G2: return "G2";
        case RocketSm_StateId_S1: return "S1";
        default: return "?";
    }
}

// Thread safe.
char const * RocketSm_event_id_to_string(RocketSm_EventId id)
{
    switch (id)
    {
        case RocketSm_EventId_EV1: return "EV1";
        case RocketSm_EventId_EV2: return "EV2";
        default: return "?";
    }
}
