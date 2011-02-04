require 'socket.io.js'

FdsDemo.gmsecController = SC.ObjectController.create
    socket: null
    subscriptions: []

    addSubscription: (subject, callback) ->
        this.subscriptions.push {subject: subject, callback: callback}
        this.socket.send {type: 'subscribe', data:subject}

    init: ->
        sc_super()
        
        # Set up our socket.io instance
        this.socket = new window.io.Socket Data_proxy.address, Data_proxy.options
        this.socket.connect()

        # Set up the listener
        this.socket.on 'message', (msg) ->
            if msg.type != 'subscribe'
                return
            msg = msg.data

            for subscription in FdsDemo.gmsecController.subscriptions
                if subscription.subject == msg.Subject
                    subscription.callback msg

        this.invokeOnce 'registerSubscriptions'

    registerSubscriptions : ->
        # Register subscriptions for each controller
        FdsDemo.notifyController.notify 'Register Subscriptions','All events registered.'

        FdsDemo.gmsecController.addSubscription 'GMSEC.FDS_DEMO.WEB.DATA.MANEUVER_PLANNING_ITERATION',              (msg) -> FdsDemo.gmsecController.processManeuverData           msg
        FdsDemo.gmsecController.addSubscription 'GMSEC.FDS_DEMO.MANEUVERING.SERVICE_REQUEST.MANEUVER_PLAN.RESULTS', (msg) -> FdsDemo.gmsecController.processManeuverPlannedResults msg
        #this.addSubscription 'GMSEC.FDS_DEMO.MANEUVER_RECONSTRUCTION.RECONSTRUCTION_DATA',       (msg) -> this.processManeuverReconstructionResults msg
        #this.addSubscription 'GMSEC.FDS_DEMO.MANEUVER_CALIBRATION.CALIBRATION_DATA',             (msg) -> this.processManeuverCalibrationResults    msg

    processManeuverData : (msg) ->

        FdsDemo.notifyController.notify 'Maneuver','Iteration registered.'

        # Extract out the maneuver ID
        if msg['Fields' ]['MANEUVER_NUMBER']['Value'] > FdsDemo.maneuversArrayController.maneuverCount
            maneuverID = FdsDemo.maneuversArrayController.addManeuver()
        else
            maneuverID = msg['Fields']['MANEUVER_NUMBER']['Value']

        iterationID = msg['Fields']['ITERATION_NUMBER']['Value']

        # Create the iteration object for the data
        maneuverIteration = SC.Object.create
        maneuverIteration.label = "Iteration #{iterationID}"
        maneuverIteration.data = []

        # Extract out the data from the message
        for field,value of msg['Fields']
            if field == 'MANEUVER_NUMBER' or field == 'ITERATION_NUMBER'
                continue
            maneuverIteration.data.push [parseFloat(field), value['Value']]

        # Sort the data based on the epoch
        maneuverIteration.data.sort (a,b) -> a[0]-b[0]

        # Subtract out the first epoch from the rest and convert to minutes
        maneuverIteration.data = ( [(datum[0]-maneuverIteration.data[0][0])*12*60, datum[1]] for datum in maneuverIteration.data )

        # Add data from iteration to plot
        iterationID = FdsDemo.maneuversArrayController.addManeuverIteration maneuverID, 'Iteration #(iterationID)'  
        FdsDemo.maneuversArrayController.addDataToIteration maneuverID, iterationID, maneuverIteration

    processManeuverPlannedResults : (msg) ->
        maneuverID = msg['Fields']['MANEUVER_NUMBER']['Value']
        dsma = msg['Fields']['MANEUVER_DSMA']['VALUE']
        console.log "DSMA: #{JSON.stringify dsma}"

        # If this is a new maneuver, add it to the properties list
        foundProperty = false
        for prop in FdsDemo.maneuversArrayController.content[maneuverID-1].dataProperties
            if prop.property == 'Delta SMA'
                prop.plannedValue = dsma 
                foundProperty = true
        if foundProperty == false
            FdsDemo.maneuversArrayController.content[maneuverID-1].dataProperties.push SC.Object.create
                property: 'Delta SMA'
                plannedValue: dsma
                reconstructionValue: 0
                calibrationValue: 0
