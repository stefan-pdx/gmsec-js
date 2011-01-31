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
        alert 'Registering'
        # Register subscriptions for each controller
        FdsDemo.notifyController.notify 'Register Subscriptions','All events registered.'
        FdsDemo.gmsecController.addSubscription 'GMSEC.FDS_DEMO.WEB.DATA.MANEUVER_PLANNING_ITERATION', (msg) -> FdsDemo.gmsecController.processManeuverData msg

    processManeuverData : (msg) ->

        FdsDemo.notifyController.notify 'Maneuver','Iteration registered.'

        # Extract out the maneuver ID
        if msg['Fields' ]['MANEUVER_NUMBER'] > FdsDemo.maneuversArrayController.maneuverCount
            maneuverID = FdsDemo.maneuversArrayController.addManeuver
        else
            maneuverID = msg['Fields']['MANEUVER_NUMBER']

        iterationID = msg['Fields']['ITERATION_NUMBER']

        # Create the iteration object for the data
        maneuverIteration = SC.Object.create 
        maneuverIteration.label = 'Iteration #(iterationID)'
        maneuverIteration.data = []

        # Extract out the data from the message
        for field in msg['Fields']
            if field == 'MANEUVER_NUMBER' or field == 'ITERATION_NUMBER'
                continue
            maneuverIteration.data.push field, msg['Fields'][field]

        # Sort the data based on the epoch
        maneuverIteration.data.sort (a,b) -> a[0]-b[0]

        # Subtract out the first epoch from the rest and convert to minutes
        maneuverIteration.data = ( [(datum[0]-maneuverIteration.data[0][0])*12*60, datum[1]] for datum in maneuverIteration.data )

        # Add data from iteration to plot
        iterationID = FdsDemo.maneuversArrayController.addManeuverIteration maneuverID, 'Iteration #(iterationID)'  
        FdsDemo.maneuversArrayController.addDataToIteration maneuverID, iterationID, maneuverIteration
