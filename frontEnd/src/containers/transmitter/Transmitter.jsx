import React, { useState } from 'react';

import { Input, Slider, Button, Typography, Box } from '@mui/joy/';




function Transmitter() {
  const [slider1Value, setSlider1Value] = useState(25000);
  const [slider2Value, setSlider2Value] = useState(1300);
  const [textInputValue, setTextInputValue] = useState("");


  const handleSlider1Change = (event, newValue) => {
    setSlider1Value(newValue);

  };

  const handleSlider2Change = (event, newValue) => {
    setSlider2Value(newValue);

  };

  const handleTextInputChange = (event) => {
    setTextInputValue(event.target.value);
  };

  const handleSubmit = async (event) => {
    event.preventDefault();
    console.log("Slider 1 value:", slider1Value);
    console.log("Slider 2 value:", slider2Value);
    console.log("Text input value:", textInputValue);
    var obj = {
      attributes: {
       // msg : "value1",
        //filename: props.name,
        //content: "value1"
        srcPath: "textInputValue",
        filePacket : 0,
        blockSize : 0
    
        
      }
    }

   obj.attributes.srcPath = textInputValue
   obj.attributes.filePacket = slider1Value
   obj.attributes.blockSize = slider2Value

    const a = obj 

    const body = { a }
    const response = await fetch('http://localhost:5000/Transmitter', {
      method: 'POST',
      headers: { "content-type": "application/json" },
      body: JSON.stringify(body)
    });

    const res = await response.json()
    console.log(res);
    // Send data to server or perform other actions here
  };

  return (
    <Box sx={{ display: 'flex', justifyContent: 'center', width: '50%', flexDirection: 'column', marginLeft: 'auto', marginRight: 'auto' }}
    >
      <Typography id="slider1" gutterBottom>
        Slider 1
      </Typography>
      <Slider
        defaultValue={25000}

        value={slider1Value}
        onChange={handleSlider1Change}
        valueLabelDisplay={"on"}
        min={500}
        max={100000}
        step={500}
        aria-labelledby="slider1"
        size='lg'
      />
      <Typography id="slider2" gutterBottom>
        Slider 2
      </Typography>
      <Slider
        defaultValue={1300}
         value={slider2Value}
        onChange={handleSlider2Change}
        valueLabelDisplay={"on"}
        min={100}
       max={Math.min(slider1Value - 100, 9500)}
        step={100}
        //max={9500}
        size='lg'
        aria-labelledby="slider2"
      />
      <Input
        placeholder='Type in here...'
        id="text-input"
        label="Text input"
        value={textInputValue}
        onChange={handleTextInputChange}
        margin="normal" />
      <Button variant="contained" onClick={handleSubmit} color="danger">
        Submit
      </Button>
    </Box>
  );
}


export default Transmitter;
