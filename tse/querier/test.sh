# ENGS 50 Module 6 Testing Script F20
# Author Brandon Guzman


~/ENGS50/Module4/tse/querier/query ./ # number of arg test 

~/ENGS50/Module4/tse/querier/query ./ indexnm # not valid crawler dir test

~/ENGS50/Module4/tse/querier/query pages file   # unreadable file test 

valgrind ~/ENGS50/Module4/tse/querier/query pages indexnm < error-queries   # prof's error tests

valgrind ~/ENGS50/Module4/tse/querier/query pages indexnm -q good-queries.txt myoutput  # run quietly test with professor's queries 



