
IMPORT mylib
MAIN
	DEFINE l_stat BOOLEAN
	DEFINE l_msg VARCHAR(80)
	CALL lock("test") RETURNING l_stat, l_msg
	IF showStat( l_stat, l_msg ) THEN
		DISPLAY "Sleep ..."
		SLEEP 5
		CALL unlock("test") RETURNING l_stat, l_msg
		IF showStat( l_stat, l_msg ) THEN
		END IF
	END IF
END MAIN
--------------------------------------------------------------------------------
FUNCTION showStat( l_stat BOOLEAN, l_msg STRING ) RETURNS BOOLEAN
	IF l_stat THEN
		DISPLAY SFMT("Okay: %1", l_msg)
		RETURN TRUE
	ELSE
		DISPLAY SFMT("Failed: %1", l_msg)
		RETURN FALSE
	END IF
END FUNCTION
