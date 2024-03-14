echo ""
echo "## Test: proj-node (localhost:3001)"
cassowary run -u "http://127.0.0.1:3001/get_first_values?n=5000" -c 3 -n 1000

echo ""
echo "## Test: proj-bun (localhost:3005)"
cassowary run -u "http://127.0.0.1:3005/get_first_values?n=5000" -c 3 -n 1000

echo ""
echo "## Test: proj-go (localhost:3002)"
cassowary run -u "http://127.0.0.1:3002/get_first_values?n=5000" -c 3 -n 1000

echo ""
echo "## Test: proj-rust (localhost:3003)"
cassowary run -u "http://127.0.0.1:3003/get_first_values?n=5000" -c 3 -n 1000

echo ""
echo "## Test: proj-cpp (localhost:3004)"
cassowary run -u "http://127.0.0.1:3004/get_first_values?n=5000" -c 3 -n 1000
