##What do you need to run this node?
 
To run this node you need to have 3 files:
 * "checkpoint" with this content as example:
```
    model_checkpoint_path: "model_epoch500.ckpt"
    all_model_checkpoint_paths: "model_epoch496.ckpt"
    all_model_checkpoint_paths: "model_epoch497.ckpt"
    all_model_checkpoint_paths: "model_epoch498.ckpt"
    all_model_checkpoint_paths: "model_epoch499.ckpt"
    all_model_checkpoint_paths: "model_epoch500.ckpt";
```
 * "*.ckpt.data-00000-of-00001", for example, model_epoch500.ckpt.data-00000-of-00001.
 This file is generated after training NN.
 
 * "*.ckpt.index",for example, model_epoch500.ckpt.index;
 This file is generated after training NN.
 
 These files together perceives as "*.ckpt"-file by tensorlow and must be stored in one
 directory.