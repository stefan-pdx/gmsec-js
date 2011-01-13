FdsDemo.maneuverController = SC.ObjectController.create
   contentBinding: SC.Binding.single 'FdsDemo.maneuversArrayController.selection'

FdsDemo.maneuversArrayController = SC.ArrayController.create
   content: [{
              epoch: 'Maneuver 1'
              data: [
                      SC.Object.create
                         label: 'set1'
                         data: [[1,2]
                                [2,3]
                                [3,4]],
                      SC.Object.create
                         label: 'zxcv'
                         data: [[0,0]
                                [1,3]
                                [2,4]]]
             },{
              epoch: 'Maneuver 2'
              data: [
                      SC.Object.create
                         label: 'set1'
                         data: [[1,4]
                                [2,3]
                                [3,1]],
                      SC.Object.create
                         label: 'zxcv'
                         data: [[0,2]
                                [1,1]
                                [2,6]]]}]
 
