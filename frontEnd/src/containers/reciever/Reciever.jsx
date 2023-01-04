/*import React from 'react'
import { TextForm } from '../../components';


const Reciever = () => {
  return (
    <div>
      <h1>Reciever</h1>
      <TextForm name="RecvConf.json" />

    </div>
  )
}

export default Reciever*/

import React, { useState } from 'react';

export default function Receiver() {
  const [destination, setDestination] = useState('');

  const handleDestinationChange = (event) => {
    setDestination(event.target.value);
  };

  const handleSubmit = () => {
    fetch('http://localhost:5000/receive', {
      method: 'POST',
      body: JSON.stringify({ destination }),
      headers: {
        'Content-Type': 'application/json',
      },
    })
    .then(response => {
      // handle success
      setDestination('');
    })
    .catch(error => {
      // handle error
    });
  };

  return (
    <div>
      <input type="text" value={destination} onChange={handleDestinationChange} />
      <button onClick={handleSubmit}>Submit</button>
    </div>
  );
}

