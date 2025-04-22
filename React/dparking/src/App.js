import React, { useEffect, useState } from 'react';
import { database, ref, get } from './firebase';
import './App.css';

function App() {
  const [availability, setAvailability] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  const fetchParkingSpotAvailability = async () => {
    const spotRef = ref(database, 'parking_spots/spot_1');
    try {
      const snapshot = await get(spotRef);
      if (snapshot.exists()) {
        setAvailability(snapshot.val().available_spot ? "Available" : "Occupied");
        setError(null); // Clear previous errors if successful
      } else {
        setAvailability("No data available");
      }
    } catch (err) {
      setError("Error fetching data: " + err.message);
      console.error("Error reading from Firebase:", err);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchParkingSpotAvailability(); // initial fetch
    const interval = setInterval(fetchParkingSpotAvailability, 1000); // fetch every 1 seconds

    return () => clearInterval(interval); // cleanup on unmount
  }, []);

  return (
    <div className="app-container">
      <div className="card">
        <h1 className="title">🚗 Parking Spot Status</h1>
        {loading ? (
          <p className="status loading">Loading...</p>
        ) : error ? (
          <p className="status error">{error}</p>
        ) : (
          <p className={`status ${availability === 'Available' ? 'available' : 'occupied'}`}>
            Spot 1: {availability}
          </p>
        )}
      </div>
    </div>
  );
}

export default App;
