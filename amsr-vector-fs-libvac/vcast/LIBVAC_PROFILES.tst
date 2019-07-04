-- VectorCAST 6.4t (05/31/17)
-- Test Case Script
-- 
-- Environment    : LIBVAC_PROFILES
-- Unit(s) Under Test: reactor
-- 
-- Script Features
TEST.SCRIPT_FEATURE:C_DIRECT_ARRAY_INDEXING
TEST.SCRIPT_FEATURE:CPP_CLASS_OBJECT_REVISION
TEST.SCRIPT_FEATURE:MULTIPLE_UUT_SUPPORT
TEST.SCRIPT_FEATURE:MIXED_CASE_NAMES
TEST.SCRIPT_FEATURE:STANDARD_SPACING_R2
TEST.SCRIPT_FEATURE:OVERLOADED_CONST_SUPPORT
TEST.SCRIPT_FEATURE:UNDERSCORE_NULLPTR
TEST.SCRIPT_FEATURE:FULL_PARAMETER_TYPES
TEST.SCRIPT_FEATURE:STRUCT_DTOR_ADDS_POINTER
TEST.SCRIPT_FEATURE:STATIC_HEADER_FUNCS_IN_UUTS
--

-- Test Case: (cl)vac::io::Reactor::RegisterEventHandler.004
TEST.UNIT:reactor
TEST.SUBPROGRAM:(cl)vac::io::Reactor::RegisterEventHandler
TEST.NEW
TEST.NAME:(cl)vac::io::Reactor::RegisterEventHandler.004
TEST.VALUE:reactor.<<GLOBAL>>.(cl).vac::io::Reactor.vac::io::Reactor.<<constructor>>.Reactor().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_handler.vac::io::EventHandler.<<constructor>>.EventHandler().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_type_mask:2
TEST.END

-- Test Case: (cl)vac::io::Reactor::RegisterEventHandler.005
TEST.UNIT:reactor
TEST.SUBPROGRAM:(cl)vac::io::Reactor::RegisterEventHandler
TEST.NEW
TEST.NAME:(cl)vac::io::Reactor::RegisterEventHandler.005
TEST.VALUE:reactor.<<GLOBAL>>.(cl).vac::io::Reactor.vac::io::Reactor.<<constructor>>.Reactor().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_handler.vac::io::EventHandler.<<constructor>>.EventHandler().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_type_mask:18
TEST.END

-- Test Case: (cl)vac::io::Reactor::RegisterEventHandler.006
TEST.UNIT:reactor
TEST.SUBPROGRAM:(cl)vac::io::Reactor::RegisterEventHandler
TEST.NEW
TEST.NAME:(cl)vac::io::Reactor::RegisterEventHandler.006
TEST.VALUE:reactor.<<GLOBAL>>.(cl).vac::io::Reactor.vac::io::Reactor.<<constructor>>.Reactor().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_handler.vac::io::EventHandler.<<constructor>>.EventHandler().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_type_mask:4
TEST.END

-- Test Case: (cl)vac::io::Reactor::RegisterEventHandler.007
TEST.UNIT:reactor
TEST.SUBPROGRAM:(cl)vac::io::Reactor::RegisterEventHandler
TEST.NEW
TEST.NAME:(cl)vac::io::Reactor::RegisterEventHandler.007
TEST.VALUE:reactor.<<GLOBAL>>.(cl).vac::io::Reactor.vac::io::Reactor.<<constructor>>.Reactor().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_handler.vac::io::EventHandler.<<constructor>>.EventHandler().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_type_mask:20
TEST.END

-- Test Case: CheckEventHdlerNullPtrBehavior.001
TEST.UNIT:reactor
TEST.SUBPROGRAM:(cl)vac::io::Reactor::RegisterEventHandler
TEST.NEW
TEST.NAME:CheckEventHdlerNullPtrBehavior.001
TEST.VALUE:reactor.<<GLOBAL>>.(cl).vac::io::Reactor.vac::io::Reactor.<<constructor>>.Reactor().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.handle:1
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_handler:<<null>>
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_type_mask:0
TEST.END

-- Test Case: CheckReadEventWerror.003
TEST.UNIT:reactor
TEST.SUBPROGRAM:(cl)vac::io::Reactor::RegisterEventHandler
TEST.NEW
TEST.NAME:CheckReadEventWerror.003
TEST.VALUE:reactor.<<GLOBAL>>.(cl).vac::io::Reactor.vac::io::Reactor.<<constructor>>.Reactor().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_handler.vac::io::EventHandler.<<constructor>>.EventHandler().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_type_mask:17
TEST.END

-- Test Case: CheckkReadEventWOerror.002
TEST.UNIT:reactor
TEST.SUBPROGRAM:(cl)vac::io::Reactor::RegisterEventHandler
TEST.NEW
TEST.NAME:CheckkReadEventWOerror.002
TEST.VALUE:reactor.<<GLOBAL>>.(cl).vac::io::Reactor.vac::io::Reactor.<<constructor>>.Reactor().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_handler.vac::io::EventHandler.<<constructor>>.EventHandler().<<call>>:0
TEST.VALUE:reactor.(cl)vac::io::Reactor::RegisterEventHandler.event_type_mask:1
TEST.END
