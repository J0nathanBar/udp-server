import React, { useState, useEffect } from 'react';

import { Input, Slider, Button, Typography, Box, FormHelperText } from '@mui/joy/';
import { NavBar } from '../../components';




function Transmitter() {
  const [slider1Value, setSlider1Value] = useState(25000);
  const [slider2Value, setSlider2Value] = useState(1300);
  const [textInputValue, setTextInputValue] = useState("");
  const [isTx, setTx] = useState(false);
  const [isLoading, setIsLoading] = useState(true)

  const handleSlider1Change = (event, newValue) => {
    setSlider1Value(newValue)

  };

  const handleSlider2Change = (event, newValue) => {
    setSlider2Value(newValue)

  };

  const handleTextInputChange = (event) => {
    setTextInputValue(event.target.value)
  };
  async function loadData() {
    try {
      const response = await fetch('http://localhost:5000/isTxRunning')
      const data = await response.json()
      console.log(data.isTx)
      setTx(data.isTx)
      setIsLoading(false)
    } catch (error) {
      console.error('Error fetching data:', error)
    }
  }

  const stopTx = async (event) => {
    setIsLoading(true)
    fetch('http://localhost:5000/stopTX', { method: 'POST' })
      .then(res => res.json())
      .then(data => {
        if (data.success) {

        }
      });
    loadData()

  }


  useEffect(() => {
    loadData();
  }, []);

  const handleSubmit = async (event) => {
    event.preventDefault()
    setIsLoading(true)

    console.log("Slider 1 value:", slider1Value)
    console.log("Slider 2 value:", slider2Value)
    console.log("Text input value:", textInputValue)
    var obj = {
      attributes: {
        srcPath: "textInputValue",
        filePacket: 0,
        blockSize: 0

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
    loadData()

  };

  return (
    <div>
      <NavBar />
      <Typography gutterBottom level='h1' color='primary' textAlign={'center'}>
        Welcome to Transmitter Configuration Settings
      </Typography>
      <Typography level='h2' color='info' textAlign={'center'}>
        Please Enter your source directory, the size of each chunk of file and the size of the data block for the encoding
      </Typography>

      <Box className="container" sx={{ display: 'flex', justifyContent: 'flex-start', width: '75%', flexDirection: 'column', marginLeft: 'auto', marginRight: 'auto' }}
      >


        <form onSubmit={(event) => {
          event.preventDefault();
          handleSubmit(event);
        }}>
          <Typography id="slider1" gutterBottom level='h3' color='warning'>
            File Chunk Size:
          </Typography>
          <Slider
            color='info'
            defaultValue={25000}
            style={{ width: 1000 }}
            value={slider1Value}
            onChange={handleSlider1Change}
            valueLabelDisplay={"on"}
            min={500}
            max={100000}
            step={500}
            aria-labelledby="slider1"
            size='lg'
          />
          <Typography id="slider2" gutterBottom level='h3' color='warning'>
            Data Block Size:
          </Typography>
          <Slider
            color='info'
            defaultValue={1300}
            value={slider2Value}
            onChange={handleSlider2Change}
            valueLabelDisplay={"on"}
            min={100}
            max={Math.min(slider1Value - 100, 9500)}
            step={100}
            size='lg'
            aria-labelledby="slider2"
          />
          <Input
            sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}
            required
            color='warning'
            variant='soft'
            placeholder='Insert Source directory here'
            id="text-input"
            label="Text input"
            value={textInputValue}
            onChange={handleTextInputChange}
            margin="normal" />
          <FormHelperText

            sx={{ width: "50%", alignSelf: "center", marginLeft: "20%", marginTop: "150px" }}
          >an example for a path could be: /home/jonny/Desktop/project/</FormHelperText>

          <Button
            sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}
            loading={isLoading}
            type='submit'
            color="success">
            Start Transmission!
          </Button>
          <Button
            disabled={!isTx}
            loading={isLoading}
            sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}
            onClick={stopTx}
            color="danger">
            Stop Transmission!
          </Button>
        </form>
      </Box>
    </div>
  );
}

export default Transmitter;
