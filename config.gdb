set breakpoint pending on
set confirm off
file ./dmc
break drewno_mars::Err::report
commands
	where
end
break drewno_mars::InternalError::InternalError
commands
	where
end

define p3
  set args p3_tests/$arg0.dm -p --
  run
end
