<?php

/**
 * @file
 * This file contains the Rules hooks and functions necessary to make the order
 * related entity, conditions, events, and actions work.
 */

/**
 * Implements hook_rules_data_info().
 */
function uc_buhsoft_rules_data_info() {
    $types['uc_order'] = array(
        'label' => t('Ubercart order object'),
        'wrap' => TRUE,
        'group' => t('Ubercart'),
    );

    return $types;
}

/**
 * Implements hook_rules_event_info().
 */
function uc_buhsoft_rules_event_info() {
    $events['uc_buhsoft_online_checkout'] = array(
        'label' => t('Buhsoft online checkout'),
        'group' => t('Order'),
        'variables' => array(
            'order' => array(
                'type' => 'uc_order',
                'label' => t('Ubercart order'),
            ),
        ),
    );

    return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function uc_buhsoft_rules_action_info() {

    $order_arg = array(
        'type' => 'uc_order',
        'label' => t('Order'),
    );

    $actions['uc_buhsoft_online_checkout'] = array(
        'label' => t('Buhsoft online checkout'),
        'group' => t('Order'),
        'base' => 'uc_buhsoft_online_checkout',
        'parameter' => array(
            'order' => $order_arg
        ),
    );

    return $actions;
}