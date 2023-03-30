import React, { useState } from 'react'
import { Input, Button, Typography, Box } from '@mui/joy/';



const Reciever = () => {
  const [textInputValue, setTextInputValue] = useState("");

  const handleTextInputChange = (event) => {
    setTextInputValue(event.target.value);
  };
  const handleSubmit = async (event) => {
    event.preventDefault();

    console.log("Text input value:", textInputValue);
    var obj = {
      attributes: {   
        srcPath: "textInputValue",   
      }
    }
   obj.attributes.srcPath = textInputValue
    const a = obj 
    const body = { a }
    const response = await fetch('http://localhost:5000/Reciever', {
      method: 'POST',
      headers: { "content-type": "application/json" },
      body: JSON.stringify(body)
    });
    const res = await response.json()
    console.log(res);
  };
  return (
    <Box sx={{ display: 'flex', justifyContent: 'center', width: '50%', flexDirection: 'column', marginLeft: 'auto', marginRight: 'auto' }}
    >
      <Typography id="slider1" gutterBottom>
        Welcome to the reciever end
      </Typography>
      <Input
        placeholder='Enter destination path in here...'
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

export default Reciever



